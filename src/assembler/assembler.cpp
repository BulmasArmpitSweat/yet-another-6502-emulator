#include "include.cpp"
#include "tables.cpp"
#include "tokenize.cpp"
#include "macro.cpp"
#include "value_literal.cpp"
#include "helper_routines.cpp"
#include "args.cpp"

#include "lib/endian/include/boost/endian.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <ios>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cstdint>

static std::string buffer;
static std::string original_buffer;

static std::vector<std::string> lines;

static std::vector<struct macro> macros;

static int position = 0;

/**
 * @brief Assemble the contents of input_file_path into a binary at output_file_path.
 * @param argc The number of command line arguments passed to the program.
 * @param argv The command line arguments passed to the program.
 * @details
 * This function is the main entry point of the assembler. It first parses the command
 * line arguments, and then reads the contents of the input file. It then processes
 * the input file, and writes the assembled binary to the output file. If any errors
 * are encountered during assembly, it prints an error message and exits.
 */

#ifdef __cplusplus
extern "C" {
#endif
void assemble(int argc, char** argv) {
    std::string input_file_path;
    std::string output_file_path;

    bool verbose;
    bool quiet;

    struct args arg = parse(argc, argv);

    input_file_path = arg.input;
    output_file_path = arg.output;
    verbose = arg.verbose;
    quiet = arg.quiet;

    if (arg.assembler_args.size() > 0) {
        for (std::string arg : arg.assembler_args) {
            if (assembler_options.find(arg) != assembler_options.end())
                assembler_options[arg] = true;
            else
                warning("Unknown assembler option: " + arg + ". Ignoring.");
        }
    }

    try {
        input_file.open(input_file_path, std::ios::in);
        output_file.open(output_file_path, std::ios::binary | std::ios::out | std::ios::app);
        token_file.open(output_file_path + ".tok", std::ios::out | std::ios::app);
    } catch (const std::ios_base::failure& err) {
        exception(err, "assemble().open()");
        wontreturn;
    }

    buffer = input_file.read();
    std::cout << buffer << std::endl;
    std::string temp;
    for (char ch : buffer) {
        ch = std::toupper(ch);
        temp += ch;
    }
    buffer = temp;
    for (char ch : temp) {
        if (!std::isspace(ch))
            temp += ch;
    }
    buffer = temp;
    buffer = buffer_cleanup(buffer);
    lines = lineify(buffer);
    macros = find_macros(lines);
    lines = strip_macros(lines);
    circular_macro_call_test(macros);
    for (int i = 0; i < macros.size(); i++)
        macros[i].lines = resolve_arguments(macros[i]);
    lines = handle_macros(lines, macros);

    // First pass: handle labels, assembler options, and variables
    int idx = 0;
    for (std::string line : lines) {
        boost::algorithm::trim(line);

        if (!line.empty() && line[0] == '#') {
            std::string option;
            std::istringstream iss(line);

            std::getline(iss, option, '=');
            std::getline(iss, option, '\0');

            // Trim leading and trailing whitespace
            boost::algorithm::trim(option);

            if (assembler_options.find(option) != assembler_options.end())
                assembler_options[option] = true;
            else
                warning_linenum(idx + 1, ("Unknown assembler option '" + option + "'; Ignoring"));
            lines[idx] = "\n";
            idx++;
            continue;
        }

        if (line.find(":=") != std::string::npos) {
            size_t pos = line.find(":=");
            std::string name;
            std::string value;

            name = line.substr(0, pos);
	    boost::algorithm::trim(name);

            value = line.substr(pos + 2);
	    boost::algorithm::trim(value);

            // Escape special characters in 'name' for regex
            std::string escapedName = std::regex_replace(name, std::regex("[.^$|()\\[\\]{}*+?\\\\]"), "\\$&");
            
            std::regex line_replace_pattern("(?<!\\w)" + escapedName + "(?!\\w)");

            for (int i = idx; i < lines.size(); ++i)
                // Replace all occurrences of the variable's name with it's value
                lines[i] = std::regex_replace(lines[i], line_replace_pattern, value);
        }

        if (line.find(":") != std::string::npos) {
            size_t pos = line.find(":");
            std::string name;

            name = line.substr(0, pos);
            boost::algorithm::trim(name); // Remove leading and trailing whitespace

            // Replace all occurrences of the label for the rest of the file
            for (int i = idx; i < lines.size(); ++i) {
                std::stringstream stream;
                stream 
                    << std::setfill('0')
                    << std::setw(4)
                    << std::hex
                    << position;
                
                std::string hex_position = stream.str();

                size_t start_pos = lines[i].find(name);
                if (start_pos != std::string::npos)
                    lines[i].replace(start_pos, name.length(), "$" + hex_position);
            }

            // Do the same backwards, for forward jumps
            for (int i = idx; i > 0; --i) {
                std::stringstream stream;
                stream 
                    << std::setfill('0')
                    << std::setw(4)
                    << std::hex
                    << position;
                
                std::string hex_position = stream.str();

                size_t start_pos = lines[i].find(name);
                if (start_pos != std::string::npos)
                    lines[i].replace(start_pos, name.length(), "$" + hex_position);
            }
            lines[idx] = '\n';
        }

        

        Addr_Modes mode = get_addr_mode(line, idx + 1);
        if (mode != Addr_Modes::UNKNOWN)
            position += size_in_bytes[mode];
        else {
            idx++;
            continue;
        }

        // For some fucky reason, this second lookup makes branches and jumps work.
        position += size_in_bytes[mode];
        idx++;
    }

    // Second pass: handle tokenization, evaluation, instruction lookup, and bytecode output
    idx = 0;
    for (std::string line : lines) {
        std::string mnemonic;
        Addr_Modes addr;
        std::string arg;
        int argtype;

        std::vector<Token> tokens;
        std::tuple<std::string, Addr_Modes, std::string, int> line_representation;

        tokens = tokenize(line, idx + 1);
        for (const Token token : tokens)
        if (tokens.size() == 1 && tokens[0].type == TokenType::_EOF) {
            idx++;
            continue;
        }

        struct line line_dat {
            .mnemonic = mnemonic,
            .mode = addr,
            .argument = arg,
            .argtype = argtype
        };

        line_representation = evaluate_line(original_buffer, line, tokens, idx + 1);

        mnemonic = std::get<TUPLE_MNEMONIC>(line_representation);
        addr = std::get<TUPLE_ADDR_MODE>(line_representation);
        arg = std::get<TUPLE_ARG>(line_representation);
        argtype = std::get<TUPLE_ARGTYPE>(line_representation);

        // NOTE: passing a reference to a class is a good way to get around disabled copy semantics
        try {
            try_write_tokens(tokens, line_dat, token_file, idx + 1);
        } catch (failstate err) {
            delete_intermediate_files(output_file, token_file);
            exception_linenum(*err, idx + 1, "assemble().write(token)");
            wontreturn;
        }

        int table_idx = 0;

        struct instruction instr;
        if (instruction_info.find(mnemonic) != instruction_info.end())
            instr = instruction_info[mnemonic];
        else {
            std::stringstream error_message;
            error_message
                << "Illegal Addressing Mode. Instruction '"
                << mnemonic
                << "' does not support the '"
                << addrmodesToString(addr)
                << "' addressing mode: "
                << join_tokens(tokens);
            
            delete_intermediate_files(output_file, token_file);
            error_linenum(idx + 1, error_message.str());
            wontreturn;
        }

        for (int i = 0; i < instr.modes.size(); i++) {
            if (instr.modes.at(i) == addr) {
                table_idx = i;
                break;
            }
        }

        if (instr.is_undocumented[table_idx] == true && assembler_options["NO-UNDOCUMENTED-INSTRUCTION-WARNING"] == false) {
            std::stringstream warning_message;
            warning_message
                << "Instruction '"
                << mnemonic
                << "' with addr mode '"
                << addrmodesToString(addr)
                << "' Is undocumented and thus likely unstable. Use with caution: "
                << join_tokens(tokens, true);
            
            warning_linenum(idx + 1, warning_message.str());
        }
        
        try {
            output_file.stream.write(reinterpret_cast<const char *>(&instr.bytecodes[table_idx]), sizeof(instr.bytecodes[table_idx]));
        } catch (const std::exception& err) {
            delete_intermediate_files(output_file, token_file);
            exception_linenum(err, idx + 1, "assemble().write(output)");
            wontreturn;
        }

        if (arg != "") {
            int value = convert_value_literal(strip_value_literal(arg), evaluate_value_literal(arg));
        

            std::vector<std::string> branches {
                "BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS" };

            if (addr == Addr_Modes::ABSOLUTE && std::find(branches.begin(), branches.end(), mnemonic) != branches.end()) {
                int offset = std::round(value / 2) - (position + 2);
                if (offset < INT8_MIN || offset > INT8_MAX) {
                    delete_intermediate_files(output_file, token_file);
                    error_linenum(idx + 1, "offset for " + mnemonic + " branch is out of range (INT8_MIN <> INT8_MAX); offset: " + std::to_string(offset));
                    wontreturn;
                }
                output_file.stream.write(reinterpret_cast<const char *>(&offset), sizeof(offset));
                goto skip_iteration;
            }

            if (strip_value_literal(arg).length() == 2 && argtype == 16) {
                delete_intermediate_files(output_file, token_file);
                std::stringstream error_message;
                std::string mode;
                if (addr == Addr_Modes::ABSOLUTE && std::find(branches.begin(), branches.end(), mnemonic) != branches.end())
                    mode = "RELATIVE";
                else
                    mode = addrmodesToString(addr);

                error_message
                    << "Attempting to pass an 8-bit value into an instruction that only accepts 16-bit values ("
                    << mnemonic
                    << " : "
                    << mode
                    << "): "
                    << join_tokens(tokens, true);
                
                error_linenum(idx + 1, error_message.str());
                wontreturn;
            } else if (strip_value_literal(arg).length() == 4 && argtype == 8) {
                delete_intermediate_files(output_file, token_file);
                std::stringstream error_message;
                std::string mode;
                if (addr == Addr_Modes::ABSOLUTE && std::find(branches.begin(), branches.end(), mnemonic) != branches.end())
                    mode = "RELATIVE";
                else
                    mode = addrmodesToString(addr);

                error_message
                    << "Attempting to pass a 16-bit value into an instruction that only accepts 8-bit values ("
                    << mnemonic
                    << " : "
                    << mode
                    << "): "
                    << join_tokens(tokens, true);
                
                error_linenum(idx + 1, error_message.str());
                wontreturn;
            }

            if (argtype != 0) {
                if (argtype == 8) {
                        output_file.stream.write(reinterpret_cast<const char *>(&value), sizeof(const char *));
                } else if (argtype == 16) {
                    boost::endian::little_int16_t val = value;
                    output_file.stream.write(reinterpret_cast<const char *>(&val), 2);
                    /*
                    uint8_t lo = static_cast<uint8_t>(value & 0xFF);
                    uint8_t hi = static_cast<uint8_t>((value >> 8) & 0xFF);
                    
                    output_file.stream.write(reinterpret_cast<const char *>(&lo), sizeof(lo));
                    output_file.stream.write(reinterpret_cast<const char *>(&hi), sizeof(hi));
                    */
                }
            }
        }
    skip_iteration:
        position += size_in_bytes[addr];
        idx++;
    }
    if (assembler_options["KEEP-TEMPS"] == false) {
        token_file.delete_file();
    }
    exit(EXIT_SUCCESS);
    wontreturn;
}
#ifdef __cplusplus
}
#endif

int main(int argc, char **argv) {
    assemble(argc, argv);
    return 0;
}

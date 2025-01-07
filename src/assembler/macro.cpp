#pragma once

#include "include.cpp"
#include "helper_routines.cpp"

#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

struct macro {
    int position;
    std::string name;
    std::string argument;
    std::vector<std::string> lines;
};

/// \brief Check if a line starts with ".macro".
///
/// \param line The line to check.
///
/// \returns true if the line starts with ".macro", false otherwise.
static inline bool is_macro_line(std::string& line) {
    return (line.rfind(".macro", 0) == 0);
}

/// \brief Check if a line starts with ".endmacro".
///
/// \param line The line to check.
///
/// \returns true if the line starts with ".endmacro", false otherwise.
static inline bool is_macro_endline(std::string& line) {
    return (line.rfind(".endmacro", 0) == 0);
}

/// \brief Split a string into a vector of strings.
///
/// Splits a string \p arguments into a vector of strings, with the delimiter \p delimiter.
/// The delimiter is not included in the output strings.
///
/// \param arguments The string to split.
/// \param delimiter The delimiter to split the string with.
///
/// \returns A vector of strings containing the split arguments.
static inline std::vector<std::string> split_arguments(std::string& arguments, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(arguments);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

/// \brief Find all macros in a file and return them in a vector.
///
/// This function goes through a vector of strings and finds all the macros in it.
/// It returns a vector of struct macro, which contains the name of the macro, its
/// argument, and the position of the macro in the source file.
///
/// Macros cannot be nested. Each macro must have a matching .endmacro.
/// Macros must have a name and a ':' only if an optional argument is intended to be passed.
///
/// If any of these rules are broken, an error is printed and the program aborts.
static std::vector<struct macro> find_macros(std::vector<std::string>& lines) {
    int linenum = 0;
    std::vector<struct macro> macros;
    struct macro head;
    bool inmacro = false;
    for (std::string& line : lines) {
        if (is_macro_endline(line) && inmacro == false) {
            delete_intermediate_files(output_file, token_file);
            error_linenum(linenum, "Found macro ending indentifier without matching macro block");
            wontreturn;
        }
        if (is_macro_line(line) && inmacro == true) {
            delete_intermediate_files(output_file, token_file);
            error_linenum(linenum, "Macros cannot be nested. Did you forget a '.endmacro'?");
            wontreturn;
        }
            
        if (is_macro_line(line) && inmacro == false) {
            if (line.find(':') != std::string::npos) {
                std::string name = line.substr(0, line.find(':'));
                std::string argument = line.substr(line.find(':') + 1);
                 if (name.empty()) {
                    delete_intermediate_files(output_file, token_file);
                    error_linenum(linenum, "Expected macro name after '.macro'");
                    wontreturn;
                 }
                if (argument.empty()) {
                    delete_intermediate_files(output_file, token_file);
                    error_linenum(linenum, "Expected argument after ':' identifier");
                    wontreturn;
                }
                head.name = name;
                head.argument = argument;
                head.position = linenum;
            }
            inmacro = true;
        }
        if (inmacro == true) {
            head.lines.push_back(line);
        }

        if (is_macro_endline(line) && inmacro == true) {
            macros.push_back(head);
            // Clear out head for new macro
            struct macro new_head;
            head = new_head;
        }
        linenum++;
    }
    if (inmacro == true) {
        delete_intermediate_files(output_file, token_file);
        error_linenum(linenum, "Unexpected EOF when looking for end of macro. Did you forget an '.endmacro'?");
        wontreturn;
    }
    return macros;
}

/// \brief Resolves argument placeholders in macro lines.
///
/// This function processes the lines of a given macro and substitutes
/// argument placeholders in the format of "$$x" with the corresponding
/// arguments provided in the macro. Each placeholder is indexed by a number,
/// where "$$1" refers to the first argument, "$$2" to the second, and so on.
/// The function ensures that all argument references are well-formed and
/// within the valid range of provided arguments.
///
/// \param macro The macro structure containing the lines and arguments to be processed.
///
/// \returns A vector of strings where all argument placeholders have been resolved.

static std::vector<std::string> resolve_arguments(struct macro macro) {
    std::vector<std::string> lines = macro.lines;
    std::vector<std::string> arguments = split_arguments(macro.argument, ',');

    for (std::string& line : lines) {
        size_t position = line.find("$$");
        while (position != std::string::npos) {
            // Ensure there is a character after "$$"
            if (position + 2 < line.size() && std::isdigit(line[position + 2])) {
                size_t end = position + 2;
                while (end < line.size() && std::isdigit(line[end])) {
                    ++end; // Find the end of the numeric index
                }

                // Extract index (1-based) and convert to integer
                int idx = std::stoi(line.substr(position + 2, end - (position + 2))) - 1;

                // Validate index range
                if (idx < 0 || idx >= static_cast<int>(arguments.size())) {
                    delete_intermediate_files(output_file, token_file);
                    error_linenum(macro.position, "Argument index out of range in macro.");
                    wontreturn;
                }

                // Replace $$x with the corresponding argument
                line.replace(position, end - position, arguments[idx]);
            } else {
                delete_intermediate_files(output_file, token_file);
                error_linenum(macro.position, "Malformed argument reference in macro.");
                wontreturn;
            }

            // Find the next occurrence of "$$" after the replacement
            position = line.find("$$", position);
        }
    }
    return lines;
}

/// \brief Inserts the contents of a macro into a vector of assembly lines.
///
/// This function takes a vector of assembly lines, a macro structure, and an
/// index position, and inserts the contents of the macro into the vector of
/// lines at the specified position. The macro is inserted as a single
/// instruction jump to a label, followed by a jump to an ("end_" + name) label
/// (which marks the end of the macro), followed by the macro itself, and
/// finally followed by a return instruction. The macro is inserted at the
/// specified position in the vector of lines, and the indices of all lines
/// following the insertion point are incremented by the number of lines in
/// the macro.
static std::vector<std::string> insert_macro(std::vector<std::string>& lines, struct macro macro, int position) {
    std::vector<std::string> temp = lines;
    std::string name = macro.name.substr(macro.name.find_first_not_of('.'));
    int idx_offset = 0;
    temp[position] = "JSR " + name;
    idx_offset++;
    temp.insert(temp.begin() + (position + idx_offset++), "JMP end_" + name);
    temp.insert(temp.begin() + (position + idx_offset++), name + ":");
    for (std::string& line : macro.lines) {
        temp.insert(temp.begin() + (position + idx_offset++), line);
    }
    temp.insert(temp.begin() + (position + idx_offset++), "RTS");
    temp.insert(temp.begin() + (position + idx_offset), "end_" + name + ":");

    return temp;
}

/// \brief Removes macro definitions from a list of assembly lines.
///
/// This function processes a list of assembly lines and removes any lines 
/// that are part of macro definitions. It identifies the start and end 
/// of macro definitions using specific line indicators and excludes 
/// these lines from the result. The function assumes that macro 
/// definitions are enclosed between lines identified by 
/// is_macro_line and is_macro_endline functions.
///
/// \param lines A vector of strings representing the assembly lines 
/// that may contain macro definitions.
///
/// \returns A vector of strings with all macro definitions removed.

static std::vector<std::string> strip_macros(std::vector<std::string>& lines) {
    std::vector<std::string> result;
    bool inmacro = false;
    for (std::string& line : lines) {
        if (is_macro_line(line) == true)
            inmacro = true;
        if (is_macro_endline(line) == true)
            inmacro = false;
        if (inmacro == false)
            result.push_back(line);
    }
    return result;
}
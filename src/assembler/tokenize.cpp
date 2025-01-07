#pragma once

#include "include.cpp"
#include "tables.cpp"
#include "value_literal.cpp"
#include "helper_routines.cpp"

#include "lib/algorithm/include/boost/algorithm/string/trim.hpp"
#include "lib/regex/include/boost/regex/v5/regex_iterator.hpp"

#include <cctype>
#include <vector>
#include <string>
#include <tuple>


// We can be pretty confident that the first three characters will constitute the opcode,
// As variables, labels and the like have been removed at this point. If not, we can be sure
// That the coder has fucked up somehow, and we can throw an error.
/**
 * @brief Tokenize a line from the input file.
 *
 * @param line The line to tokenize.
 * @param linenum The line number of the line in the input file.
 *
 * @returns A vector of tokens.
 *
 * @details Each token is a tuple of a TokenType and a string. The string is the
 * part of the line that corresponds to the token. The TokenType can be used to
 * determine the type of the token.
 *
 * @note The line is trimmed before tokenization, so any whitespace at the
 * beginning or end of the line is ignored.
 *
 * @note The function does not check if the line is well-formed. It will return
 * a vector of tokens even if the line is not a valid instruction.
 */
static std::vector<Token> tokenize(const std::string& line, const int linenum) {
    std::string temp = line;
    boost::algorithm::trim(temp);
    int off = 0;
    std::vector<Token> tokens;
    char mnemonic[3];
    mnemonic[0] = line[off++];
    mnemonic[1] = line[off++];
    mnemonic[2] = line[off++];
    if (std::isalpha(mnemonic[0]) && std::isalpha(mnemonic[1]) && std::isalpha(mnemonic[2])) {
        if (mnemonics.find(mnemonic) != mnemonics.end()) {
            tokens.emplace_back(TokenType::MNEMONIC, mnemonic);
        } else {
            delete_intermediate_files(output_file, token_file);
            error_linenum(linenum, ("Unknown mnemonic: " + std::string(mnemonic)));
            wontreturn;
        }
    } else {
        delete_intermediate_files(output_file, token_file);
        error_linenum(linenum, ("Unknown mnemonic: " + std::string(mnemonic)));
        wontreturn;
    }
    for (; off < line.length(); off++) {
        if (line[off] == 'A') {
            tokens.emplace_back(TokenType::ACCUMULATOR, "A");
        
        } else if (line[off] == 'X') {
            tokens.emplace_back(TokenType::MNEMONIC, "X");
        
        } else if (line[off] == 'Y') {
            tokens.emplace_back(TokenType::MNEMONIC, "Y");
        
        } else if (line[off] == '$') {
            std::string buf;
            buf.push_back(line[off++]);
            while (std::isalnum(line[off]))
                buf.push_back(line[off++]);
            tokens.emplace_back(TokenType::LITERAL, buf);

        } else if (line[off] == '#') {
            tokens.emplace_back(TokenType::HASH, "#");
        
        } else if (line[off] == '(') {
            tokens.emplace_back(TokenType::OPENING_BRACKET, "(");
        
        } else if (line[off] == ')') {
            tokens.emplace_back(TokenType::CLOSING_BRACKET, ")");
        
        } else if (line[off] == ',') {
            tokens.emplace_back(TokenType::COMMA, ",");
        
        } else if (line[off] == ':') {
            if (line[off + 1] == '=')
                tokens.emplace_back(TokenType::CONST_EQUALS, ":=");
            else
                tokens.emplace_back(TokenType::COLON, ":");
        
        } else if (line[off] == '=') {
                tokens.emplace_back(TokenType::EQUALS, '=');
        
        } else {
            tokens.emplace_back(TokenType::UNKNOWN, line[off]);
        }
    }
    tokens.emplace_back(TokenType::_EOF, "EOF");
    return tokens;
}

/**
 * @brief Compile a vector of Tokens into a vector of strings, where each string is the TokenType of the corresponding Token.
 *
 * @param tokens The vector of Tokens to compile.
 *
 * @returns A vector of strings.
 *
 * @details This function is used by evaluate_line to compare the token types of a line to the token patterns of the addressing modes.
 */
static inline std::vector<std::string> compile_token_types(const std::vector<Token>& tokens) {
    std::vector<std::string> types;
    for (Token token : tokens)
        types.push_back(token.type);
    return types;
}

/**
 * @brief Evaluate a line of 6502 assembly and return a tuple of the mnemonic, addressing mode, argument, and number of bytes.
 *
 * @param original_buffer The original source code file.
 *
 * @param line The line of 6502 assembly to evaluate.
 *
 * @param tokens The vector of Tokens that the line has been split into.
 *
 * @param linenum The line number of the line in the source code file.
 *
 * @returns A tuple of the mnemonic, addressing mode, argument, and number of bytes.
 *
 * @details This function takes a line of 6502 assembly and a vector of Tokens that the line has been split into, and evaluates it to determine the mnemonic, addressing mode, argument, and the number of bytes the instruction takes up. If the line is invalid, it will print an error message and exit.
 */
static std::tuple<std::string, Addr_Modes, std::string, int> evaluate_line(std::string& original_buffer, std::string line, std::vector<Token> tokens, int linenum) {
    for (const auto& [mode, pattern] : addressing_mode_patterns) {
        if (compile_token_types(tokens) == pattern) {
            if (mode == Addr_Modes::BASIC_ARGUMENT) {
                std::string argument = tokens[literal_position[mode]].value;

                if (strip_value_literal(argument).length() == 4)
                    return std::tuple(tokens[0].value, Addr_Modes::ABSOLUTE, argument, 16);

                else if (strip_value_literal(argument).length() == 2)
                    return std::tuple(tokens[0].value, Addr_Modes::ZERO_PAGE, argument, 8);

                else {
                    delete_intermediate_files(output_file, token_file);
                    error_linenum(linenum, ("Unknown addressing mode " + join_tokens(tokens)));
                    wontreturn;
                }
            }

            if (literal_position[mode] != 0)
                return std::tuple(tokens[0].value, mode, tokens[literal_position[mode]].value, arg_types[mode]);
            else
                return std::tuple(tokens[0].value, mode, "", arg_types[mode]);
        } else
            continue;
    }
    delete_intermediate_files(output_file, token_file);
    error_linenum(linenum, ("Unknown addressing mode " + join_tokens(tokens)));
    wontreturn;
}

/**
 * Determines the addressing mode of a given assembly line.
 *
 * @param line The assembly line as a string.
 * @param linenum The line number in the source code file.
 * @returns The addressing mode identified from the line.
 *
 * @details This helper function tokenizes the input assembly line, compares the token types against predefined 
 * addressing mode patterns, and returns the corresponding addressing mode. If an argument is present, 
 * it checks its length to further classify it as ABSOLUTE or ZERO_PAGE. If no known pattern matches, 
 * it returns Addr_Modes::UNKNOWN.
 */

static Addr_Modes get_addr_mode(const std::string& line, const int linenum) {
    std::vector<Token> tokens = tokenize(clean_line(line), linenum);
    for (const auto& [mode, pattern] : addressing_mode_patterns) {
        if (compile_token_types(tokens) == pattern) {
            std::string argument = tokens[literal_position[mode]].value;
            if (strip_value_literal(argument).length() == 4)
                return Addr_Modes::ABSOLUTE;
            else if (strip_value_literal(argument).length() == 2)
                return Addr_Modes::ZERO_PAGE;
            return mode;
        }
    }
    return Addr_Modes::UNKNOWN;
}
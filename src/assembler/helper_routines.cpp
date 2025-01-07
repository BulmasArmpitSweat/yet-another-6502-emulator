#pragma once

#include <cstdarg>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <unicode/urename.h>
#include <vector>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <iostream>

#include "lib/regex/include/boost/regex/v5/regex.hpp"
#include "include.cpp"

static failstate try_write_tokens(std::vector<Token> tokens, struct line& line, file_handle* output_file, int linenum) {
    try {
        output_file->stream
            << "Tokens for line "
            << linenum
            <<":"
            << std::endl;
        
        for (Token tok : tokens)
            output_file->stream 
                << tok.to_string() 
                << std::endl;
        
        output_file->stream
            << "Internal representation for line "
            << linenum
            << ":"
            << std::endl;
        
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&line);
        std::size_t size = sizeof(line);

        for (std::size_t i = 0; i < size; ++i) {
            output_file->stream 
                << std::hex 
                << std::uppercase 
                << std::setfill('0') 
                << std::setw(2) 
                << static_cast<int>(bytes[i]) 
                << ' ';
        }
        output_file->stream 
            << std::dec 
            << std::endl; // Reset to decimal output after printing
    } catch (failstate e) {
        throw e;
    }
    return nullptr;
}

static std::string buffer_cleanup(const std::string& buffer) {
    boost::regex pattern(R"((\w+:)(\s*[A-Za-z]))");
    return boost::regex_replace(buffer, pattern, R"(\1\n\2)");
}

static std::vector<std::string> line_split(const std::string& buffer) {
    std::vector<std::string> return_list;
    std::istringstream stream(buffer);

    std::string line;

    while (std::getline(stream, line)) {
        return_list.push_back(line);
    }

    return return_list;
}

static std::vector<std::string> lineify(const std::string& buffer) {
    boost::regex commentRegex(R"(//[^\n]*|/\*.*?\*/|;[^\n]*)");
    std::string cleanedBuffer = boost::regex_replace(buffer, commentRegex, "");
    std::vector<std::string> lines = line_split(cleanedBuffer);

    return lines;
}

/**
 * Removes all whitespace characters from the input string.
 *
 * @param line The input string to be cleaned of whitespace.
 * @return A new string with all whitespace characters removed from the input.
 */

static std::string clean_line(const std::string& line) {
    std::string return_line;
    for (char ch : line) {
        if (!std::isspace(ch))
            return_line.push_back(ch);
    }
    return return_line;
}

static std::string join_tokens(const std::vector<Token>& tokens, bool strip_eof = false) {
    std::ostringstream oss;
    for (const Token& token : tokens) {
        if (strip_eof == true) {
            if (token.type == TokenType::_EOF)
                return oss.str();
        }
        oss << " " + token.value;
    }
    return oss.str();
}

template <typename... Files>
static inline void delete_intermediate_files(Files&&... files) {
    if (assembler_options["KEEP-OUTPUT-ON-FAIL"])
        (..., files->delete_file());
}
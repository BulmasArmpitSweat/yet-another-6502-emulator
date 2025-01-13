#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

#define wontreturn std::exit(EXIT_FAILURE)

class file_handle;

/* C++ only supports pointers for variables using forward-declared classes */
file_handle* input_file;
file_handle* output_file;
file_handle* token_file;

typedef char byte;
typedef unsigned char byte_raw;
typedef std::exception* failstate;

class TokenType {
public:
    static const std::string ACCUMULATOR;
    static const std::string MNEMONIC;
    static const std::string LITERAL;
    static const std::string HASH;
    static const std::string OPENING_BRACKET;
    static const std::string CLOSING_BRACKET;
    static const std::string COMMA;
    static const std::string CONST_EQUALS;
    static const std::string EQUALS;
    static const std::string COLON;
    static const std::string UNKNOWN;
    static const std::string _EOF;
};

const std::string TokenType::ACCUMULATOR = "ACCUMULATOR";
const std::string TokenType::MNEMONIC = "MNEMONIC";
const std::string TokenType::LITERAL = "LITERAL";
const std::string TokenType::HASH = "HASH";
const std::string TokenType::OPENING_BRACKET = "OPENING_BRACKET";
const std::string TokenType::CLOSING_BRACKET = "CLOSING_BRACKET";
const std::string TokenType::COMMA = "COMMA";
const std::string TokenType::CONST_EQUALS = "CONST_EQUALS";
const std::string TokenType::EQUALS = "EQUALS";
const std::string TokenType::COLON = "COLON";
const std::string TokenType::UNKNOWN = "UNKNOWN";
const std::string TokenType::_EOF = "EOF";

enum class bcolours : byte {
    HEADER,
    OKBLUE,
    OKCYAN,
    OKGREEN,
    WARNING,
    FAIL,
    ENDC,
    BOLD,
    UNDERLINE
};

std::unordered_map<bcolours, std::string> bcolorsStrings = {
    {bcolours::HEADER, "\033[95m"},
    {bcolours::OKBLUE, "\033[94m"},
    {bcolours::OKCYAN, "\033[96m"},
    {bcolours::OKGREEN, "\033[92m"},
    {bcolours::WARNING, "\033[93m"},
    {bcolours::FAIL, "\033[91m"},
    {bcolours::ENDC, "\033[0m"},
    {bcolours::BOLD, "\033[1m"},
    {bcolours::UNDERLINE, "\033[4m"}
};

static inline std::string bcoloursToString(bcolours colour) {
    return bcolorsStrings.at(colour);
}

class Token {
public:
    std::string type;
    std::string value;

    Token(std::string in_type, const char* in_value)
        : type(in_type),
          value(in_value) {};

    Token(std::string in_type, const char in_value)
        : type(in_type) {
            value.push_back(in_value);
        };

    Token(std::string in_type, std::string in_value)
        : type(in_type),
          value(in_value) {};

    std::string to_string() const {
        return "Token: " + type + "(" + value + ")";
    }
};

enum class Addr_Modes : byte {
    UNKNOWN,
    IMPLIED,
    ACCUMULATOR,
    IMMEDIATE,
    ABSOLUTE,
    X_INDEXED_ABSOLUTE,
    Y_INDEXED_ABSOLUTE,
    ABSOLUTE_INDIRECT,
    ZERO_PAGE,
    X_INDEXED_ZERO_PAGE,
    Y_INDEXED_ZERO_PAGE,
    X_INDEXED_ZERO_PAGE_INDIRECT,
    ZERO_PAGE_INDIRECT_Y_INDEXED,
    BASIC_ARGUMENT,
};

std::unordered_map<Addr_Modes, std::string> addrmodeStrings = {
    {Addr_Modes::UNKNOWN, "UNKNOWN"},
    {Addr_Modes::IMPLIED, "IMPLIED"},
    {Addr_Modes::ACCUMULATOR, "ACCUMULATOR"},
    {Addr_Modes::IMMEDIATE, "IMMEDIATE"},
    {Addr_Modes::ABSOLUTE, "ABSOLUTE"},
    {Addr_Modes::X_INDEXED_ABSOLUTE, "X_INDEXED_ABSOLUTE"},
    {Addr_Modes::Y_INDEXED_ABSOLUTE, "Y_INDEXED_ABSOLUTE"},
    {Addr_Modes::ABSOLUTE_INDIRECT, "ABSOLUTE_INDIRECT"},
    {Addr_Modes::ZERO_PAGE, "ZERO_PAGE"},
    {Addr_Modes::X_INDEXED_ZERO_PAGE, "X_INDEXED_ZERO_PAGE"},
    {Addr_Modes::Y_INDEXED_ZERO_PAGE, "Y_INDEXED_ZERO_PAGE"},
    {Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, "X_INDEXED_ZERO_PAGE_INDIRECT"},
    {Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED, "ZERO_PAGE_INDIRECT_Y_INDEXED"},
    {Addr_Modes::BASIC_ARGUMENT, "BASIC_ARGUMENT"},
};

static inline std::string addrmodesToString(Addr_Modes mode) {
    return addrmodeStrings.at(mode);
}

static constexpr int TUPLE_MNEMONIC = 0;
static constexpr int TUPLE_ADDR_MODE = 1;
static constexpr int TUPLE_ARG = 2;
static constexpr int TUPLE_ARGTYPE = 3;

static void warning(const std::string msg) {
    std::cerr 
        << bcoloursToString(bcolours::WARNING) 
        << "[WARN]: " 
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << msg
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
}

void warning_linenum(const int linenum, std::string msg) {
    std::cerr 
        << bcoloursToString(bcolours::WARNING) 
        << "[WARN line: "
        << linenum
        << "]: " 
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << msg
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
}

void error(std::string msg, bool crash = true) {
    std::cerr
        << bcoloursToString(bcolours::FAIL)
        << "[ERROR]: " 
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << msg
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
    if (crash)
        exit(-EXIT_FAILURE);
    wontreturn;
}

void error_linenum(int linenum, std::string msg, bool crash = true) {
    std::cerr 
        << bcoloursToString(bcolours::WARNING) 
        << "[ERROR line: "
        << linenum
        << "]: " 
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << msg
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
    if (crash)
        exit(-EXIT_FAILURE);
    wontreturn;
}

void exception(const std::exception& e, const std::string& name) {
    std::cerr
        << bcoloursToString(bcolours::FAIL)
        << bcoloursToString(bcolours::UNDERLINE)
        << "[EXCEPTION | name: "
        << name
        << "]: "
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << "An unexpected error occurred - "
        << e.what()
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
    exit(-EXIT_FAILURE);
    /* Definitely */ wontreturn;
}

void exception_linenum(std::exception e, int linenum, std::string name) {
    std::cerr
        << bcoloursToString(bcolours::FAIL)
        << bcoloursToString(bcolours::UNDERLINE)
        << "[EXCEPTION | name: "
        << name
        << "| line: "
        << linenum
        << "]: "
        << bcoloursToString(bcolours::ENDC)
        << bcoloursToString(bcolours::BOLD)
        << "An unexpected error occurred - "
        << std::string(e.what())
        << bcoloursToString(bcolours::ENDC)
        << std::endl;
    exit(-EXIT_FAILURE);
    /* Definitely */ wontreturn;
}

std::map<std::string, bool> assembler_options = {
    {"NO-UNDOCUMENTED-INSTRUCTION-WARNING", false},
    {"KEEP-TEMPS", false},
    {"KEEP-OUTPUT-ON-FAIL", false},
    {"VERBOSE", false},
    {"NO_CYCLICAL_MACRO_CALL_WARNING", false},
    
};

std::map<Addr_Modes,  std::vector<std::string>> addressing_mode_patterns = {
    {Addr_Modes::ACCUMULATOR,                  {TokenType::MNEMONIC, TokenType::ACCUMULATOR, TokenType::_EOF}},
    {Addr_Modes::IMPLIED,                      {TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::ACCUMULATOR,                  {TokenType::MNEMONIC, TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::IMMEDIATE,                    {TokenType::MNEMONIC, TokenType::HASH, TokenType::LITERAL, TokenType::_EOF}},
    {Addr_Modes::BASIC_ARGUMENT,               {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::_EOF}},
    // {Addr_Modes::ABSOLUTE,                     {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::_EOF}},
    {Addr_Modes::X_INDEXED_ABSOLUTE,           {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::COMMA, TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::Y_INDEXED_ABSOLUTE,           {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::COMMA, TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::ABSOLUTE_INDIRECT,            {TokenType::MNEMONIC, TokenType::OPENING_BRACKET, TokenType::LITERAL, TokenType::CLOSING_BRACKET, TokenType::_EOF}},
    // {Addr_Modes::ZERO_PAGE,                    {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::_EOF}},
    {Addr_Modes::X_INDEXED_ZERO_PAGE,          {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::COMMA, TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::Y_INDEXED_ZERO_PAGE,          {TokenType::MNEMONIC, TokenType::LITERAL, TokenType::COMMA, TokenType::MNEMONIC, TokenType::_EOF}},
    {Addr_Modes::X_INDEXED_ZERO_PAGE_INDIRECT, {TokenType::MNEMONIC, TokenType::OPENING_BRACKET, TokenType::LITERAL, TokenType::COMMA, TokenType::MNEMONIC, TokenType::CLOSING_BRACKET, TokenType::_EOF}},
    {Addr_Modes::ZERO_PAGE_INDIRECT_Y_INDEXED, {TokenType::MNEMONIC, TokenType::OPENING_BRACKET, TokenType::LITERAL, TokenType::CLOSING_BRACKET, TokenType::COMMA, TokenType::MNEMONIC, TokenType::_EOF}},
};

// Little additions to std namespace
namespace std {
    inline bool isnum(int c) {
        if (c > '0' && c < '9')
            return true;
        return false;
    }

    inline std::string toupperstr(std::string str) {
        std::string buf;
        for (char c : str)
            buf.push_back(std::toupper(c));
        return buf;
    }
}

struct line {
    std::string mnemonic;
    Addr_Modes mode;
    std::string argument;
    int argtype;
};

struct instruction {
    std::vector<Addr_Modes> modes;
    std::vector<unsigned char> bytecodes;
    std::vector<bool> is_undocumented;
};

class file_handle {
public:
    std::fstream stream;
    std::string filepath;

    file_handle() = default;

    file_handle(const std::string& path, std::ios_base::openmode mode = std::ios::out)
        : filepath(path), stream(path, mode) {
        if (!stream.is_open()) {
            throw std::ios_base::failure("Failed to open file: " + path);
        }
    }

    // Disable copy semantics
    file_handle(const file_handle&) = delete;
    file_handle& operator=(const file_handle&) = delete;

    // Allow move semantics
    file_handle(file_handle&& other) noexcept
        : stream(std::move(other.stream)), filepath(std::move(other.filepath)) {}

    file_handle& operator=(file_handle&& other) noexcept {
        if (this != &other) {
            stream = std::move(other.stream);
            filepath = std::move(other.filepath);
        }
        return *this;
    }

    /*
        @breif reads @n characters from file
        @return read buffer
        NOTE: if @n isn't supplied or is -1, read until eof
    */
    std::string read(int n = -1) {
        std::string output;
        char c;
        for (;;) {
            c = stream.get();
            if (c == EOF)
                break;
            output.push_back(c);
        }
        return output;
    }

    // Method to open a file
    void open(const std::string& path, std::ios_base::openmode mode = std::ios::out) {
        filepath = path;
        stream.open(path, mode);
        if (!stream.is_open()) {
            throw std::ios_base::failure("Failed to open file member: " + path);
        }
    }

    // Method to close the file
    void close() {
        if (stream.is_open()) {
            stream.close();
        }
    }

    int delete_file() {
        stream.close();
        return std::remove(filepath.c_str());
    }

    ~file_handle() {
        close();
    }
};
#pragma once

#include <cctype>
#include <string>
#include <algorithm>
#include <stdexcept>

std::string strip_value_literal(std::string literal) {
    std::string first_two = literal.substr(0, 2);
    for (char c : first_two)
        c = std::toupper(c);
    std::transform(first_two.begin(), first_two.end(), first_two.begin(), ::toupper);

    char last_char = std::toupper(literal.back());

    if (first_two == "0B" || first_two == "0X")
        return literal.substr(2);

    if (literal[0] == '$')
        return literal.substr(1);

    if (last_char == 'H' || last_char == 'D')
        return literal.substr(0, literal.size() - 1);

    return literal;
}

int evaluate_value_literal(std::string literal) {
    std::string first_two = literal.substr(0, 2);
    for (char c : first_two)
        c = std::toupper(c);
    std::transform(first_two.begin(), first_two.end(), first_two.begin(), ::toupper);

    char last_char = std::toupper(literal.back());

    if (first_two == "0B")
        return 2;

    if (literal[0] == '$' || first_two == "0X" || last_char == 'H')
        return 16;

    if (last_char == 'D')
        return 10;

    return 0;
}

int convert_value_literal(const std::string& literal, const int base) {
    // Check if the base is valid
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Base must be between 2 and 36");
    }

    // Convert the literal to an integer using the given base
    return std::stoi(literal, nullptr, base);
}

std::string convert_value_literal_back(const int value, const int base) {
    int temp = value;
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Base must be in the range [2, 36]");
    }

    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    bool is_negative = temp < 0 && base == 10; // Handle negative numbers for base 10
    temp = std::abs(temp);

    std::string result;
    do {
        result = digits[temp % base] + result;
        temp /= base;
    } while (temp > 0);

    if (is_negative) {
        result = '-' + result;
    }

    return result;
}
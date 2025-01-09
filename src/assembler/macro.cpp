#pragma once

#include "include.cpp"
#include "helper_routines.cpp"
#include "value_literal.cpp"
#include "tokenize.cpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <tuple>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <unordered_map>

std::unordered_set<std::string> currently_expanding;
std::unordered_map<std::string, std::vector<std::string>> expanded_cache;

struct macro {
    int position;
    std::string name;
    std::string argument;
    std::vector<std::string> lines;
    struct macro empty() const {
        return macro { .position = -1, .name = "", .argument = "" };
    }

    bool is_empty() {
        if (position == -1 && name == "" && argument == "")
            return true;
        return false;
    }

    bool operator==(const struct macro& other) const {
        return (position == other.position && name == other.name && lines == other.lines);
    }
};

/// \brief Check if a line starts with ".macro".
///
/// \param line The line to check.
///
/// \returns true if the line starts with ".macro", false otherwise.
static inline bool is_macro_line(const std::string& line) {
    return (line.rfind(".macro", 0) == 0);
}

/// \brief Check if a line starts with ".endmacro".
///
/// \param line The line to check.
///
/// \returns true if the line starts with ".endmacro", false otherwise.
static inline bool is_macro_endline(const std::string& line) {
    return (line.rfind(".endmacro", 0) == 0);
}

/// \brief Check if a line starts with ".call".
///
/// \param line The line to check.
///
/// \returns true if the line starts with ".call", false otherwise.
static inline bool is_macro_call_sub_instruction(const std::string& line) {
    return (line.rfind(".call", 0) == 0);
}

/// \brief Check if a macro has a nested macro .call sub_instruction.
///
/// \param macro The macro to check.
///
/// \returns true if the macro has a nested macro .call sub_instruction, false otherwise.
static inline bool macro_contains_macro_call(struct macro macro) {
    // Find if a macro has a nested macro .call sub_instruction
    for (std::string& line : macro.lines) {
        if (is_macro_call_sub_instruction(line))
            return true;
    }
    return false;
}

/// \brief Strip ".call" and the following space from a line that calls a macro.
///
/// Given a line that calls a macro (i.e. a line that starts with ".call"), this
/// function strips the ".call" and the following space from the string and
/// returns the resulting string.
static inline std::string strip_call_sub_instruction(const std::string& line) {
    std::string temp = line;
    temp = temp.substr(5);
    return temp;
}

static inline struct macro find_macro_in_table(std::vector<struct macro>& macros, const struct macro& target) {
    for (const struct macro& macro : macros) {
        if (macro == target)
            return target;
    }
    return target.empty();
}

static inline struct macro find_macro_in_table_from_name(const std::vector<struct macro>& macros, const std::string& target) {
    for (const struct macro& macro : macros) {
        if (macro.name == target)
            return macro;
    }
    return macros[0].empty();
}

/// \brief Extract macro information from a line that calls a macro.
///
/// Given a line that calls a macro (i.e. a line that starts with ".call"), this
/// function extracts the name of the macro and its argument (if any) and packs
/// them into a struct macro.
///
/// \param calling_line The line that calls the macro.
/// \param linenum The line number of the calling line.
///
/// \returns A struct macro with the extracted information.
static inline struct macro get_macro_info_from_call(const std::string& calling_line, int linenum) {
    std::string temp = calling_line;
    temp = clean_line(temp);
    struct macro macro;

    if (temp.empty())
        return macro.empty();


    /* Length of '.call' subinstruction, implying that nothing was passed to it */
    if (temp.length() == 5) {
        delete_intermediate_files(output_file, token_file);
        error_linenum(linenum, "Expected macro name and optional arguments after call subinstruction");
        wontreturn;
    }

    temp = temp.erase(5); // Erase '.call' from start
    if (temp.find(':') != std::string::npos) {
        macro.name = temp.substr(0, temp.find(':'));
    } else {
        macro.name = temp; // Line contains nothing else but the name at this point
        macro.argument = "";
        macro.position = -1; // Position isn't needed here
        return macro;
    }

    macro.argument = temp.substr(temp.find(':'), temp.length());
    macro.position = -1;
    return macro;
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

/// \brief Build a directed graph of macro calls.
///
/// Given a vector of macro structures, this function builds a directed graph of
/// macro calls. Each node in the graph represents a macro, and each edge
/// represents a call from one macro to another.
///
/// \param macros The vector of macro structures to build the graph with.
///
/// \returns A map of macro names to vectors of macro names, where each key in
/// the map is a macro name and the value associated with it is a vector of
/// macro names that it calls.
static inline std::map<std::string, std::vector<std::string>> build_macro_graph(const std::vector<struct macro>& macros) {
    std::map<std::string, std::vector<std::string>> graph;
    for (const struct macro& macro : macros) {
        for (const std::string& line : macro.lines) {
            if (is_macro_call_sub_instruction(line)) {
                std::string called_macro = strip_call_sub_instruction(line);
                graph[macro.name].push_back(called_macro);
            }
        }
    }
    return graph;
}

/// \brief Detect a cycle in a directed macro graph.
///
/// \param node The node to check.
/// \param graph The graph to check.
/// \param visited A set of visited nodes.
/// \param recursion_stack A set of nodes in the current recursion stack.
/// \param path A vector of nodes to store the path of the cycle.
///
/// \returns true if a cycle is detected, false otherwise.
static inline bool detect_cycle(const std::string& node, const std::map<std::string, std::vector<std::string>>& graph,
                                std::set<std::string>& visited, std::set<std::string>& recursion_stack,
                                std::vector<std::string>& path) {
    visited.insert(node);
    recursion_stack.insert(node);
    path.push_back(node);

    for (const std::string& neighbour : graph.at(node)) {
        if (recursion_stack.find(neighbour) != recursion_stack.end()) {
            // Cycle detected, print the path
            std::cerr
                << bcoloursToString(bcolours::WARNING) 
                << "[WARN]: " 
                << bcoloursToString(bcolours::ENDC)
                << bcoloursToString(bcolours::BOLD)
                << "Potential circular macro call detected: ";
            auto itr = std::find(path.begin(), path.end(), neighbour);
            for (; itr != path.end(); ++itr) {
                std::cerr 
                    << *itr 
                    << " -> ";
            }
            std::cerr 
                << neighbour 
                << " <- loops back to "
                << neighbour
                << bcoloursToString(bcolours::ENDC)
                << "\n";
            return true;
        }

        if (visited.find(neighbour) == visited.end()) {
            if (detect_cycle(neighbour, graph, visited, recursion_stack, path)) {
                return true;
            }
        }
    }

    recursion_stack.erase(node);
    path.pop_back();
    return false;
}


    /**
     * Checks if there is a circular macro call in the list of macros.
     * 
     * @param macros The list of macros to check.
     * 
     * @return true if a circular macro call is detected, false otherwise.
     */
static bool circular_macro_call_test(std::vector<struct macro> macros) {
    // Base case
    if (macros.size() < 2)
        return false;

    auto graph = build_macro_graph(macros);
    std::set<std::string> visited;
    std::set<std::string> recursion_stack;
    std::vector<std::string> path;

    for (const auto& [macro_name, _] : graph) {
        if (visited.find(macro_name) == visited.end()) {
            if (detect_cycle(macro_name, graph, visited, recursion_stack, path)) {
                return true;
            }
        }
    }
    return false;
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
static inline std::vector<struct macro> find_macros(std::vector<std::string>& lines) {
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

static inline std::vector<std::string> expand_macro(const std::string& macro_name, const struct macro& macro, const std::vector<struct macro>& macros, int linenum) {
    if (currently_expanding.count(macro_name)) {
        delete_intermediate_files(output_file, token_file);
        error_linenum(linenum, "Infinite macro call cycle in macro: " + macro_name);
        wontreturn;
    }

    currently_expanding.insert(macro_name);

    std::vector<std::string> expanded_lines;
    for (const std::string& line : macro.lines) {
        if (is_macro_call_sub_instruction(line)) {
            std::string called_macro_name = get_macro_info_from_call(line, linenum).name;
            if (std::find(macros.begin(), macros.end(), called_macro_name) != macros.end()) {
                std::vector<std::string> sub_expansion = expand_macro(called_macro_name, find_macro_in_table_from_name(macros, called_macro_name), macros, linenum);
                expanded_lines.insert(expanded_lines.end(), sub_expansion.begin(), sub_expansion.end());
            }
        } else {
            expanded_lines.push_back(line);
        }
    }

    currently_expanding.erase(macro_name);
    return expanded_lines;
}

static inline std::vector<std::string> expand_macro_with_cache(const std::string& macro_name, const struct macro& macro, const std::vector<struct macro>& macros, int linenum) {
    if (expanded_cache.count(macro_name)) {
        return expanded_cache[macro_name];
    }

    std::vector<std::string> expanded_lines = expand_macro(macro_name, macro, macros, linenum);
    expanded_cache[macro_name] = expanded_lines;
    return expanded_lines;
}

static inline std::pair<bool, int> can_reuse_macro(const struct macro& target_macro, const std::vector<std::string>& lines, int current_line_idx, int max_offset_forwards, int max_offset_backwards) {
    int offset = 0;
    for (int i = current_line_idx; i >= 0; i--) {
        if (is_macro_call_sub_instruction(lines[i])) {
            std::string called_macro_name = get_macro_info_from_call(lines[i], current_line_idx).name;
            if (called_macro_name == target_macro.name) {
                // can use it, offset
                return std::pair(true, -i);
            } else {
                Addr_Modes mode = get_addr_mode(lines[i], current_line_idx);
                offset += size_in_bytes[mode];
                if (offset > max_offset_backwards) {
                    break;
                }
            }
        }
    }
    offset = 0;
    for (int i = current_line_idx; i < lines.size(); i++) {
        if (is_macro_call_sub_instruction(lines[i])) {
            std::string called_macro_name = get_macro_info_from_call(lines[i], current_line_idx).name;
            if (called_macro_name == target_macro.name) {
                // can use it, offset
                return std::pair(true, i);
            } else {
                Addr_Modes mode = get_addr_mode(lines[i], current_line_idx);
                offset += size_in_bytes[mode];
                if (offset > max_offset_forwards) {
                    break;
                }
            }
        }
    }

    return std::pair(false, 0);
}

std::pair<std::string, std::vector<std::string>> define_macro_subroutine(const std::string& macro_name, const std::vector<std::string>& expanded_lines, int linenum) {
    std::tuple<std::string, std::vector<std::string>> output;
    std::string subroutine_label = "macro_" + macro_name;
    std::vector<std::string> output_code;
    output_code.push_back(subroutine_label + ":");
    output_code.insert(output_code.end(), expanded_lines.begin(), expanded_lines.end());
    output_code.push_back("RTS");
    return std::pair(subroutine_label, output_code);
}

std::vector<std::string> handle_macros(std::vector<std::string>& lines, const std::vector<struct macro>& macros) {
    int linenum = 1;
    bool foundmacro = false;
    for (; linenum < lines.size(); linenum++) {
        if (!is_macro_call_sub_instruction(lines[linenum]))
            continue;
        foundmacro = true;
        struct macro target = get_macro_info_from_call(lines[linenum], linenum);
        if (std::find(macros.begin(), macros.end(), target) == macros.end()) {
            delete_intermediate_files(output_file, token_file);
            error_linenum(linenum, "Attempting to call to an undefined macro");
            wontreturn;
        } else if (std::find(macros.begin(), macros.end(), target)->position > linenum) {
            delete_intermediate_files(output_file, token_file);
            error_linenum(linenum, "Attempting to call to a macro that is not yet defined");
            wontreturn;
        }
        struct macro macro_to_expand = find_macro_in_table_from_name(macros, target.name);
        std::pair<bool, int> can_reuse = can_reuse_macro(macro_to_expand, lines, linenum, INT8_MAX, INT8_MIN);
        if (can_reuse.first == true) {
            lines[linenum] = "JSR $" + reverse_convert_value_literal(can_reuse.second, 16);
            break;
        } else {
            std::pair<std::string, std::vector<std::string>> subroutine = define_macro_subroutine(macro_to_expand.name, macro_to_expand.lines, linenum);
            std::vector<std::string> reversed_lines = subroutine.second;
            std::reverse(reversed_lines.begin(), reversed_lines.end());
            for (std::string line : reversed_lines)
                lines.insert(lines.begin() + linenum, line);
            break;
        }
    }
    if (foundmacro == true)
        handle_macros(lines, macros);
    return lines;
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
#pragma once

#include <vector>
#include <string>

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    result.push_back("");
    long unsigned int i = 0;
    bool in_quotes = false;
    char last_char = '\0';
    while (i < str.size())
    {
        if (last_char != '\\' && (str[i] == '"' || str[i] == '\'')) {
            in_quotes = !in_quotes;
            result[result.size() - 1] += str[i];
        } else if (str[i] == delimiter && !in_quotes) {
            result.push_back("");
        }else{
            result[result.size() - 1] += str[i];
        }
        last_char = str[i];
        ++i;
    }
    
    return result;
}
inline std::string join(const std::vector<std::string>& parts, const std::string& delimiter, int startIndex = 0, int endIndex = -1) {
    if (endIndex == -1) endIndex = parts.size();
    std::string result;
    for (int i = startIndex; i < endIndex; ++i) {
        if (i > startIndex) result += delimiter;
        result += parts[i];
    }
    return result;
}
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}
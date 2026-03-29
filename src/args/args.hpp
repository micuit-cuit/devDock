#pragma once
#include <string>
#include "../lib/str.hpp"
#include <optional>
#include <vector>

int hasOption(const std::string& option, int argc, char* argv[]);
int hasMultipleOptions(const std::string& option, int argc, char* argv[]);
std::optional<std::string> getOptionValue(const std::string& option, int argc, char* argv[], int index = 0);
std::optional<std::vector<std::string>> hasInvalidOptions(const std::vector<std::string>& options, int argc, char* argv[]);
#include <string>
#include "../lib/str.hpp"
int has_option(const std::string& option, int argc, char* argv[]);
std::string get_option_value(const std::string& option, int argc, char* argv[]);
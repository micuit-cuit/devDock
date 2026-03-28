#include "args.hpp"
int has_option(const std::string& option, int argc, char* argv[]){
    // try to find --option 
    for (int i = 1; i < argc; ++i) {
        if (std::string("--") + option == split(argv[i], '=')[0]) {
            return i;
        }
        if (std::string("-") + option == split(argv[i], '=')[0]) {
            return i;
        }
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-' && arg[1] != '-' && arg.find(option.substr(1)) != std::string::npos) {
            return i;
        }
    }
    return 0;
}
std::string get_option_value(const std::string& option, int argc, char* argv[]){
    for (int i = 1; i < argc; ++i) {
        if (std::string("--") + option == split(argv[i], '=')[0]) {
            return join(split(argv[i], '='), '=', 1);
        }
    }
    return "";
}
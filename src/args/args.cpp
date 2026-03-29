#include "args.hpp"
int hasOption(const std::string& option, int argc, char* argv[]){
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
int hasMultipleOptions(const std::string& option, int argc, char* argv[]){
    int count = 0;
    for (int i = 1; i < argc; ++i) {
        if (std::string("--") + option == split(argv[i], '=')[0]) {
            count++;
        }
        if (std::string("-") + option == split(argv[i], '=')[0]) {
            count++;
        }
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-' && arg[1] != '-' && arg.find(option.substr(1)) != std::string::npos) {
            count++;
        }
    }
    return count;
}
std::optional<std::string> getOptionValue(const std::string& option, int argc, char* argv[], int index){
    int i = 0;
    for (int j = 1; j < argc; ++j) {
        auto parts = split(argv[j], '=');
        if (parts[0] == "--" + option) {
            if (i++ != index) continue;
            // --flag=value  → retourne ce qui suit le '='
            if (parts.size() > 1)
                return join(parts, "=", 1);
            // --flag (sans '=') → retourne ""
            return "";
        }
    }
    return std::nullopt;  // option absente
}
std::optional<std::vector<std::string>> hasInvalidOptions(const std::vector<std::string>& options, int argc, char* argv[]){
    std::vector<std::string> invalids;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            std::string opt_name = split(arg, '=')[0];
            bool valid = false;
            for (const auto& valid_opt : options) {
                if (opt_name == "--" + valid_opt || opt_name == "-" + valid_opt) {
                    valid = true;
                    break;
                }
                if (opt_name[1] != '-' && opt_name.find(valid_opt.substr(1)) != std::string::npos) {
                    valid = true;
                    break;
                }
            }
            if (!valid) {
                invalids.push_back(opt_name);
            }
        }
    }
    if (!invalids.empty()) return invalids;
    return std::nullopt;
}
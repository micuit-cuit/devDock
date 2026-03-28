#include "./define.hpp"
#include "./src/help/help.hpp"
#include "./src/args/args.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) { print_help(argv[0]); return 0; }
    std::string command = argv[1];
    if (command == "help") {
        Help help;
        help.print_help(argv[0]);
        return 0;
    }else if (command == "version") {
        std::cout << "devDock version " << VERSION << "\n";
        return 0;
    }else if (command == "create") {
        std::cout << "Create a new devContainer\n";
        return 0;
    }else if (command == "ps") {
        std::cout << "List running devContainers\n";
        return 0;
    }else if (command == "run" || command == "start") {
        std::cout << "Run a command in a devContainer\n";
        return 0;
    }else if (command == "stop") {
        std::cout << "Stop a running devContainer\n";
        return 0;
    }else if (command == "shell") {
        std::cout << "Open a shell in a running devContainer\n";
        return 0;
    }else if (command == "exec") {
        std::cout << "Execute a command in a running devContainer\n";
        return 0;
    }else if (command == "rm") {
        std::cout << "Remove a devContainer\n";
        return 0;
    }
    if (has_option("help", argc, argv)) {
        std::cout << "Help requested\n";
        return 0;
    }
}
#include "./define.hpp"
#include "./help/help.hpp"
#include "./lib/log/log.hpp"
#include "./args/args.hpp"
#include "./create/create.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Log logger(__FILE__);

    if (argc < 2) { logger.error("No command provided");Help help; help.print_help(argv[0]); return 0; }
    std::string command = argv[1];
    logger.dev(CTX, "Command received", command, "with args");
    if (command == "help") {
        Help help;
        help.print_help(argv[0]);
        return 0;
    }else if (command == "version") {
        logger.info("devDock version", VERSION);
        return 0;
    }else if (command == "create") {
        logger.info("Creating devContainer");
        return 0;
    }else if (command == "ps") {
        logger.info("List running devContainers");
        return 0;
    }else if (command == "run" || command == "start") {
        logger.info("Run a command in a devContainer");
        return 0;
    }else if (command == "stop") {
        logger.info("Stop a running devContainer");
        return 0;
    }else if (command == "shell") {
        logger.info("Open a shell in a running devContainer");
        return 0;
    }else if (command == "exec") {
        logger.info("Execute a command in a running devContainer");
        return 0;
    }else if (command == "rm") {
        logger.info("Remove a devContainer");
        return 0;
    }
    if (has_option("help", argc, argv)) {
        logger.info("Help requested");
        return 0;
    }
}
#include "./define.hpp"
#include "./help/help.hpp"
#include "./lib/log/log.hpp"
#include "./args/args.hpp"
#include "./create/create.hpp"
#include "./lib/proc/proc.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Log logger(__FILE__);

    if (argc < 2) { logger.error("No command provided");Help help; help.print_help(argv[0]); return 0; }
    std::string command = argv[1];
    logger.dev(CTX, "Command received", command, "with args");
    if (command == "run") {
        // start a process to list files in current directory
        std::string cmd = get_option_value("cmd", argc, argv);
        if (cmd.empty()) cmd = "ls -l";
        std::vector<std::string> args = split(cmd, ' ');
        logger.dev(CTX, "Running command", cmd);
        int rc = Proc::run(args);
        if (rc != 0) {
            logger.error(CTX, cmd, "command failed with exit code", rc);
        }
        logger.dev(CTX, cmd, "command exited with code", rc);
        return rc;
    }else if (command == "shell") {
        std::string script = get_option_value("script", argc, argv);
        if (script.empty()) script = "echo Hello from shell!";
        logger.dev(CTX, "Running shell script", script);
        int rc = Proc::shell(script);
        if (rc != 0) {
            logger.error(CTX, "Shell command failed with exit code", rc);
        }
        logger.dev(CTX, "Shell command exited with code", rc);
        return rc;
    }else if (command == "rod") {
        std::string cmd = get_option_value("cmd", argc, argv);
        if (cmd.empty()) cmd = "ls -l";
        std::vector<std::string> args = split(cmd, ' ');
        logger.dev(CTX, "Running command with run_or_die", cmd);
        Proc::run_or_die(args, "Failed to execute command: " + cmd);
        return 0;
    }else if (command == "silent") {
        std::string cmd = get_option_value("cmd", argc, argv);
        if (cmd.empty()) cmd = "ls -l";
        std::vector<std::string> args = split(cmd, ' ');
        logger.dev(CTX, "Running command silently", cmd);
        Proc::run_silent(args);
        return 0;
    }else if (command == "capture") {
        std::string cmd = get_option_value("cmd", argc, argv);
        if (cmd.empty()) cmd = "echo Captured output";
        std::vector<std::string> args = split(cmd, ' ');
        logger.dev(CTX, "Capturing output of command", cmd);
        std::string output = Proc::capture(args);
        logger.dev(CTX, "Captured output:", output);
        return 0;
    }
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
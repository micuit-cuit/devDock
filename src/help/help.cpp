#include "help.hpp"
void Help::printHelp(const char* programName){
    std::cout << "Usage: " << programName << " <command> [options]\n";
    std::cout << "Commands:\n";
    std::cout << "  build   Build a Docker image from a Dockerfile\n";
    std::cout << "  run     Run a command in a new container\n";
    std::cout << "  ps      List containers\n";
    std::cout << "  images   List images\n";
    std::cout << "  rm      Remove a container\n";
    std::cout << "  rmi     Remove an image\n";
    std::cout << "  help    Show this help message\n";
}
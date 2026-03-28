#include <string>
#include <iostream>
int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " exit_code" << std::endl;
        return 0;
    }
    int code = std::stoi(argv[1]);
    return code;
}
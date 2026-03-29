#include "../lib/log/log.hpp"
#include "../lib/colors.hpp"
#include "../lib/utiles/utiles.hpp"
#include "../args/args.hpp"
#include <string>

class Create {
    private:
        Log logger = Log(__FILE__);
    public:
        Create(int argc, char** argv);
        void create_container(const std::string& name, const std::string* args, int argc);
        void help(std::string processName);
};
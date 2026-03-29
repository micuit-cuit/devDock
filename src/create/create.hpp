#include "../lib/log/log.hpp"
#include "../lib/json.hpp"
#include "../lib/proc/proc.hpp"
#include "../lib/colors.hpp"
#include "../lib/utiles/utiles.hpp"
#include "../args/args.hpp"
#include <string>
#include <fstream>

class Create {
    private:
        Log logger = Log(__FILE__);
        std::string debootstrapPath();
        bool testOptions(int argc, char** argv);
        nlohmann::json parseConfigFile(std::string path, int argc, char** argv);

    public:
        Create(int argc, char** argv);
        void createContainer(const std::string& name, const std::string* args, int argc);
        void help(std::string processName);
};
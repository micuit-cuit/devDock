#include "utiles.hpp"

std::string getLang() {
    // Ordre de priorité standard POSIX
    for (const char* var : {"LANGUAGE", "LC_ALL", "LC_MESSAGES", "LANG"}) {
        const char* val = std::getenv(var);
        if (val && val[0] != '\0')
            return std::string(val);
    }
    return "en"; // fallback
}
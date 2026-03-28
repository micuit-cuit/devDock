#include "log.hpp"
#include <ctime>

// ── Log constructor ───────────────────────────────────────────────────────────
Log::Log(std::string fileName) {
    this->fileName = fileName;
}

// ── getDateTime ───────────────────────────────────────────────────────────────
std::string getDateTime() {
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%d-%m-%Y.%X", &tstruct);
    return buf;
}

// ── formatJson ────────────────────────────────────────────────────────────────
std::string formatJson(const nlohmann::json& j, int indent) {
    std::string pad(indent * 2, ' ');
    std::string pad2((indent + 1) * 2, ' ');
    std::string out;

    if (j.is_object()) {
        out += colors::white + "{\n";
        int i = 0;
        for (auto& [key, val] : j.items()) {
            out += pad2 + colors::cyan + "\"" + key + "\"" + colors::reset
                 + colors::white + ": " + colors::reset
                 + formatJson(val, indent + 1);
            if (++i < (int)j.size()) out += colors::white + ",";
            out += "\n";
        }
        out += pad + colors::white + "}";

    } else if (j.is_array()) {
        out += colors::white + "[\n";
        for (int i = 0; i < (int)j.size(); i++) {
            out += pad2 + formatJson(j[i], indent + 1);
            if (i + 1 < (int)j.size()) out += colors::white + ",";
            out += "\n";
        }
        out += pad + colors::white + "]";

    } else if (j.is_string()) {
        out += colors::green + "\"" + j.get<std::string>() + "\"" + colors::reset;
    } else if (j.is_number_integer()) {
        out += colors::yellow + std::to_string(j.get<long>()) + colors::reset;
    } else if (j.is_number_float()) {
        out += colors::yellow + std::to_string(j.get<double>()) + colors::reset;
    } else if (j.is_boolean()) {
        out += colors::magenta + (j.get<bool>() ? "true" : "false") + colors::reset;
    } else if (j.is_null()) {
        out += colors::bright_grey + "null" + colors::reset;
    }

    return out;
}
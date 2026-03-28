#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include <vector>
#include "json.hpp"
#include "colors.hpp"

#define CTX Ctx{__func__, __LINE__}

// ── Ctx ───────────────────────────────────────────────────────────────────────
struct Ctx {
    std::string function = "";
    int line = -1;
};

// ── Helpers (déclarés ici, définis dans log.cpp) ──────────────────────────────
std::string formatJson(const nlohmann::json& j, int indent = 0);
std::string getDateTime();

template<typename T>
std::string getTypeName() {
    int status;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string result = (status == 0 && demangled) ? demangled : typeid(T).name();
    free(demangled);

    static const std::vector<std::pair<std::string, std::string>> replacements = {
        { "nlohmann::json_abi_v3_12_0::basic_json<std::map, std::vector, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool, long, unsigned long, double, std::allocator, nlohmann::json_abi_v3_12_0::adl_serializer, std::vector<unsigned char, std::allocator<unsigned char> >, void>", "json" },
        { "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>", "std::string" },
        { "std::vector<", "vec<" },
        { "std::map<",    "map<" },
        { ", std::allocator<", "<" },
    };

    for (const auto& [from, to] : replacements) {
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.size(), to);
            pos += to.size();
        }
    }
    return result;
}

// ── class Log ─────────────────────────────────────────────────────────────────
class Log {
public:
    Log(std::string fileName = "");

    template<typename... Args> void dev  (Args... args)          { print(-1, Ctx{}, args...); }
    template<typename... Args> void dev  (Ctx ctx, Args... args) { print(-1, ctx,   args...); }
    template<typename... Args> void log  (Args... args)          { print( 0, Ctx{}, args...); }
    template<typename... Args> void log  (Ctx ctx, Args... args) { print( 0, ctx,   args...); }
    template<typename... Args> void info (Args... args)          { print( 1, Ctx{}, args...); }
    template<typename... Args> void info (Ctx ctx, Args... args) { print( 1, ctx,   args...); }
    template<typename... Args> void warn (Args... args)          { print( 2, Ctx{}, args...); }
    template<typename... Args> void warn (Ctx ctx, Args... args) { print( 2, ctx,   args...); }
    template<typename... Args> void error(Args... args)          { print( 3, Ctx{}, args...); }
    template<typename... Args> void error(Ctx ctx, Args... args) { print( 3, ctx,   args...); }

private:
    std::string fileName;

    void print_one(std::string&, const std::string&) {}

    template<typename T, typename... Rest>
    void print_one(std::string& output, const std::string& color, T arg, Rest... rest) {
        if constexpr (std::is_same_v<T, std::string>) {
            output += color + arg + colors::reset + " ";
        } else if constexpr (std::is_same_v<std::decay_t<T>, char*> || std::is_same_v<std::decay_t<T>, const char*>) {
            output += color + std::string(arg) + colors::reset + " ";
        } else if constexpr (std::is_same_v<T, bool>) {
            output += colors::magenta + (arg ? std::string("true") : std::string("false")) + colors::reset + " ";
        } else if constexpr (std::is_integral_v<T>) {
            output += colors::red + std::to_string(arg) + colors::reset + " ";
        } else if constexpr (std::is_floating_point_v<T>) {
            output += colors::yellow + std::to_string(arg) + colors::reset + " ";
        } else if constexpr (std::is_same_v<T, nlohmann::json>) {
            output += "\n" + formatJson(arg);
        } else {
            output += colors::green + "<" + getTypeName<T>() + ">" + colors::reset + " ";
        }
        print_one(output, color, rest...);
    }

    template<typename... Args>
    void print(const int status, const Ctx& ctx, Args... args) {
        std::string output;
        std::string color;

        switch (status) {
            case -1: color = colors::magenta; output += color + colors::bold + "[DEV] "   + colors::reset; break;
            case  0: color = colors::cyan;    output += color + colors::bold + "[LOG] "   + colors::reset; break;
            case  1: color = colors::blue;    output += color + colors::bold + "[INFO] "  + colors::reset; break;
            case  2: color = colors::yellow;  output += color + colors::bold + "[WARN] "  + colors::reset; break;
            case  3: color = colors::red;     output += color + colors::bold + "[ERROR] " + colors::reset; break;
        }

        output += colors::color<255, 165, 0>() + "{" + getDateTime() + "}" + colors::reset + " ";

        if (!fileName.empty())
            output += colors::color<0, 255, 255>() +"["+ fileName + "]"+ colors::reset + " ";

        if (ctx.line != -1 && !ctx.function.empty())
            output += colors::color<255, 105, 180>() +"("+ ctx.function + ":" + std::to_string(ctx.line) + ")"+ colors::reset + " ";
        else if (ctx.line != -1)
            output += colors::color<255, 105, 180>() + "(line " + std::to_string(ctx.line) + ")"+ colors::reset + " ";
        else if (!ctx.function.empty())
            output += colors::color<255, 105, 180>() + "(" + ctx.function + ")"+ colors::reset + " ";
        print_one(output, color, args...);
        std::cout << output << std::endl;
    }
};
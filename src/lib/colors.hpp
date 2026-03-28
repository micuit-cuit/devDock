#ifndef COLORS_HPP_
#define COLORS_HPP_

#include <string>

#if defined(_WIN32) || defined(_WIN64)
#   define COLORS_TARGET_WINDOWS
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#   define COLORS_TARGET_POSIX
#endif

#if !defined(COLORS_USE_ANSI_ESCAPE) && !defined(COLORS_USE_WINDOWS_API)
#   if defined(COLORS_TARGET_POSIX)
#       define COLORS_USE_ANSI_ESCAPE
#   elif defined(COLORS_TARGET_WINDOWS)
#       define COLORS_USE_WINDOWS_API
#   endif
#endif

#if defined(COLORS_USE_WINDOWS_API)
#   include <Windows.h>
#   include <sstream>
    namespace colors {
        inline void wset_attributes(int foreground, int background = -1) {
            static WORD defaultAttributes = 0;
            HANDLE handleTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!defaultAttributes) {
                CONSOLE_SCREEN_BUFFER_INFO info;
                if (!GetConsoleScreenBufferInfo(handleTerminal, &info)) return;
                defaultAttributes = info.wAttributes;
            }
            if (foreground == -1 && background == -1) {
                SetConsoleTextAttribute(handleTerminal, defaultAttributes);
                return;
            }
            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!GetConsoleScreenBufferInfo(handleTerminal, &info)) return;
            if (foreground != -1) {
                info.wAttributes &= ~(info.wAttributes & 0x0F);
                info.wAttributes |= static_cast<WORD>(foreground);
            }
            if (background != -1) {
                info.wAttributes &= ~(info.wAttributes & 0xF0);
                info.wAttributes |= static_cast<WORD>(background);
            }
            SetConsoleTextAttribute(handleTerminal, info.wAttributes);
        }
        // Sur Windows, les couleurs ne peuvent pas être stockées en string ANSI.
        // On retourne des strings vides et on applique les couleurs via l'API au moment de l'affichage.
        inline std::string reset()       { wset_attributes(-1, -1); return ""; }
        inline std::string bold()        { return ""; }
        inline std::string faint()       { return ""; }
        inline std::string italic()      { return ""; }
        inline std::string underline()   { return ""; }
        inline std::string blink()       { return ""; }
        inline std::string reverse_()    { return ""; }
        inline std::string invisible()   { return ""; }
        inline std::string strikethrough() { return ""; }
        inline std::string grey()        { wset_attributes(0); return ""; }
        inline std::string red()         { wset_attributes(FOREGROUND_RED); return ""; }
        inline std::string green()       { wset_attributes(FOREGROUND_GREEN); return ""; }
        inline std::string yellow()      { wset_attributes(FOREGROUND_RED | FOREGROUND_GREEN); return ""; }
        inline std::string blue()        { wset_attributes(FOREGROUND_BLUE); return ""; }
        inline std::string magenta()     { wset_attributes(FOREGROUND_RED | FOREGROUND_BLUE); return ""; }
        inline std::string cyan()        { wset_attributes(FOREGROUND_BLUE | FOREGROUND_GREEN); return ""; }
        inline std::string white()       { wset_attributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); return ""; }
        inline std::string bright_grey() { wset_attributes(0 | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_red()  { wset_attributes(FOREGROUND_RED | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_green(){ wset_attributes(FOREGROUND_GREEN | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_yellow(){ wset_attributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_blue() { wset_attributes(FOREGROUND_BLUE | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_magenta(){ wset_attributes(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_cyan() { wset_attributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY); return ""; }
        inline std::string bright_white(){ wset_attributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); return ""; }
        inline std::string on_grey()     { wset_attributes(-1, 0); return ""; }
        inline std::string on_red()      { wset_attributes(-1, BACKGROUND_RED); return ""; }
        inline std::string on_green()    { wset_attributes(-1, BACKGROUND_GREEN); return ""; }
        inline std::string on_yellow()   { wset_attributes(-1, BACKGROUND_RED | BACKGROUND_GREEN); return ""; }
        inline std::string on_blue()     { wset_attributes(-1, BACKGROUND_BLUE); return ""; }
        inline std::string on_magenta()  { wset_attributes(-1, BACKGROUND_RED | BACKGROUND_BLUE); return ""; }
        inline std::string on_cyan()     { wset_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN); return ""; }
        inline std::string on_white()    { wset_attributes(-1, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE); return ""; }
        inline std::string on_bright_grey()   { wset_attributes(-1, 0 | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_red()    { wset_attributes(-1, BACKGROUND_RED | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_green()  { wset_attributes(-1, BACKGROUND_GREEN | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_yellow() { wset_attributes(-1, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_blue()   { wset_attributes(-1, BACKGROUND_BLUE | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_magenta(){ wset_attributes(-1, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_cyan()   { wset_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY); return ""; }
        inline std::string on_bright_white()  { wset_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY); return ""; }
    }
#else
    // POSIX : les couleurs sont de simples strings ANSI
    namespace colors {
        inline const std::string reset        = "\033[00m";
        inline const std::string bold         = "\033[1m";
        inline const std::string faint        = "\033[2m";
        inline const std::string italic       = "\033[3m";
        inline const std::string underline    = "\033[4m";
        inline const std::string blink        = "\033[5m";
        inline const std::string reverse_     = "\033[7m";
        inline const std::string invisible    = "\033[8m";
        inline const std::string strikethrough= "\033[9m";
        inline const std::string grey         = "\033[30m";
        inline const std::string red          = "\033[31m";
        inline const std::string green        = "\033[32m";
        inline const std::string yellow       = "\033[33m";
        inline const std::string blue         = "\033[34m";
        inline const std::string magenta      = "\033[35m";
        inline const std::string cyan         = "\033[36m";
        inline const std::string white        = "\033[37m";
        inline const std::string bright_grey  = "\033[90m";
        inline const std::string bright_red   = "\033[91m";
        inline const std::string bright_green = "\033[92m";
        inline const std::string bright_yellow= "\033[93m";
        inline const std::string bright_blue  = "\033[94m";
        inline const std::string bright_magenta="\033[95m";
        inline const std::string bright_cyan  = "\033[96m";
        inline const std::string bright_white = "\033[97m";
        inline const std::string on_grey      = "\033[40m";
        inline const std::string on_red       = "\033[41m";
        inline const std::string on_green     = "\033[42m";
        inline const std::string on_yellow    = "\033[43m";
        inline const std::string on_blue      = "\033[44m";
        inline const std::string on_magenta   = "\033[45m";
        inline const std::string on_cyan      = "\033[46m";
        inline const std::string on_white     = "\033[47m";
        inline const std::string on_bright_grey   = "\033[100m";
        inline const std::string on_bright_red    = "\033[101m";
        inline const std::string on_bright_green  = "\033[102m";
        inline const std::string on_bright_yellow = "\033[103m";
        inline const std::string on_bright_blue   = "\033[104m";
        inline const std::string on_bright_magenta= "\033[105m";
        inline const std::string on_bright_cyan   = "\033[106m";
        inline const std::string on_bright_white  = "\033[107m";

        // Couleurs RGB personnalisées (ANSI 24-bit)
        template<int R, int G, int B>
        inline std::string color() {
            return "\033[38;2;" + std::to_string(R) + ";" + std::to_string(G) + ";" + std::to_string(B) + "m";
        }

        template<int R, int G, int B>
        inline std::string on_color() {
            return "\033[48;2;" + std::to_string(R) + ";" + std::to_string(G) + ";" + std::to_string(B) + "m";
        }
    }
#endif

#undef COLORS_TARGET_POSIX
#undef COLORS_TARGET_WINDOWS
#undef COLORS_USE_ANSI_ESCAPE
#undef COLORS_USE_WINDOWS_API

#endif
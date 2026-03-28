#include "proc.hpp"
#include "../log/log.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

namespace Proc {

// ── Helpers internes ─────────────────────────────────────────

static std::vector<const char*> to_cargs(const std::vector<std::string>& args) {
    std::vector<const char*> cargs;
    cargs.reserve(args.size() + 1);
    for (const auto& a : args)
        cargs.push_back(a.c_str());
    cargs.push_back(nullptr);
    return cargs;
}

// ── API publique ─────────────────────────────────────────────

int run(const std::vector<std::string>& args) {
    Log logger(__FILE__);
    if (args.empty()) return -1;

    pid_t pid = fork();
    if (pid < 0)
        logger.error(CTX, "fork() failed: " + std::string(strerror(errno)));
    if (pid == 0) {
        auto cargs = to_cargs(args);
        execvp(cargs[0], const_cast<char* const*>(cargs.data()));
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

int shell(const std::string& script) {
    return run({"/bin/bash", "-c", script});
}

void run_or_die(const std::vector<std::string>& args, const std::string& err_msg) {
    int rc = run(args);
    if (rc != 0) {
        Log logger(__FILE__);
        logger.error(CTX, err_msg.empty() ? ("Commande échouée : " + args[0]) : err_msg);
    }
}

void run_silent(const std::vector<std::string>& args) {
    if (args.empty()) return;

    pid_t pid = fork();
    if (pid == 0) {
        int devnull = open("/dev/null", O_WRONLY);  
        if (devnull >= 0) {
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        auto cargs = to_cargs(args);
        execvp(cargs[0], const_cast<char* const*>(cargs.data()));
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
}

std::string capture(const std::vector<std::string>& args) {
    if (args.empty()) return "";

    int pipefd[2];
    if (pipe(pipefd) < 0) return "";

    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        auto cargs = to_cargs(args);
        execvp(cargs[0], const_cast<char* const*>(cargs.data()));
        _exit(127);
    }

    close(pipefd[1]);
    std::string result;
    char buf[256];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0)
        result.append(buf, static_cast<size_t>(n));
    close(pipefd[0]);
    waitpid(pid, nullptr, 0);

    // Trim trailing newline
    while (!result.empty() && (result.back() == '\n' || result.back() == '\r'))
        result.pop_back();

    return result;
}

} // namespace Proc

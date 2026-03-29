#pragma once
#include <string>
#include <vector>

// Helpers pour lancer des sous-processus
namespace Proc {

// Lance un programme, retourne le code de retour
int run(const std::vector<std::string>& args);

// Lance un script bash (-c), retourne le code de retour
int shell(const std::string& script);

// Lance un programme, exit() si le code de retour est non-zéro
void runOrDie(const std::vector<std::string>& args, const std::string& errMsg = "");

// Lance un programme en ignorant stdout/stderr et le code de retour
void runSilent(const std::vector<std::string>& args);

// Lance un programme et capture sa sortie standard (stdout)
std::string capture(const std::vector<std::string>& args);

} // namespace Proc

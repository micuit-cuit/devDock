#include "create.hpp"

Create::Create(int argc, char** argv) {
    if (argc < 3) {
        logger.error(CTX, "No container name provided");
        help(argv[0]);
        return;
    }
    if (has_option("help", argc, argv)) {
        help(argv[0]);
        return;
    }
}

void Create::help(std::string processName) {
    std::string c_title  = colors::bold + colors::bright_cyan;
    std::string c_sect   = colors::bold + colors::bright_green;
    std::string c_opt    = colors::bright_blue;
    std::string c_arg    = colors::yellow;
    std::string c_cmt    = colors::bright_grey;
    std::string c_ex     = colors::bright_magenta;
    std::string c_star   = colors::bright_red;
    std::string r        = colors::reset;
    std::string lang = getLang().substr(0, 2);
    if (lang == "fr")
        logger.raw(
            c_title + "Usage:" + r + "\n"
            "  " + colors::bold + processName + r + " " + c_opt + "create" + r + " " + c_arg + "<container_name>" + r + " [options]\n"
            "\n"
            "  " + c_cmt + "Les options marquées d'un " + c_star + "*" + c_cmt + " peuvent être répétées pour spécifier plusieurs valeurs.\n"
            "  ex: --cmd=\"apt update\" --cmd=\"apt install -y curl\"" + r + "\n"
            "\n"

            + c_sect + "Source de l'OS :" + r + "\n"
            "  " + c_opt + "--os" + r + "=<nom>                      Image de base à utiliser           " + c_cmt + "(ex: ubuntu)" + r + "\n"
            "  " + c_opt + "--rootfs" + r + "=<chemin>               Système de fichiers racine         " + c_cmt + "(remplace --os)" + r + "\n"
            "  " + c_opt + "--dest" + r + "=<chemin>                 Destination des fichiers conteneur " + c_cmt + "(défaut: ./rootfs-os, avec --os)" + r + "\n"
            "\n"

            + c_sect + "Configuration du conteneur :" + r + "\n"
            "  " + c_opt + "--shell" + r + "=<chemin>                Shell à utiliser                  " + c_cmt + "(défaut: /bin/bash)" + r + "\n"
            "  " + c_opt + "--cmd" + r + "=<commande>         " + c_star + "*" + r + "      Commandes exécutées à l'install   " + c_cmt + "(ex: \"apt update && apt install -y curl\")" + r + "\n"
            "  " + c_opt + "--env" + r + "=<KEY=VALUE>        " + c_star + "*" + r + "      Variables d'environnement\n"
            "  " + c_opt + "--user" + r + "=<user:pass:shell> " + c_star + "*" + r + "      Utilisateur à créer               " + c_cmt + "(ex: \"alice:password:/bin/bash\")" + r + "\n"
            "  " + c_opt + "--services" + r + "=<cmd>         " + c_star + "*" + r + "      Services lancés au démarrage      " + c_cmt + "(ex: \"nginx\")" + r + "\n"
            "\n"

            + c_sect + "Réseau :" + r + "\n"
            "  " + c_opt + "--host-ip" + r + "=<ip>                 IP hôte du veth                    " + c_cmt + "(défaut: 10.88.0.1)" + r + "\n"
            "  " + c_opt + "--cont-ip" + r + "=<ip>                 IP du conteneur                    " + c_cmt + "(défaut: 10.88.0.2)" + r + "\n"
            "  " + c_opt + "--subnet" + r + "=<cidr>                Sous-réseau CIDR                   " + c_cmt + "(défaut: 10.88.0.0/24)" + r + "\n"
            "  " + c_opt + "--dns" + r + "=<ip>               " + c_star + "*" + r + "     Serveurs DNS                       " + c_cmt + "(ex: \"8.8.8.8\")" + r + "\n"
            "\n"

            + c_sect + "Autres :" + r + "\n"
            "  " + c_opt + "--help" + r + "                         Affiche cette aide\n"
            "  " + c_opt + "--config" + r + "=<chemin>              Fichier de config JSON             " + c_cmt + "(combinable, CLI prime sur le fichier)" + r + "\n"
            "  " + c_opt + "--generate-config" + r + "              Génère un fichier de config JSON   " + c_cmt + "(ne crée pas le conteneur)" + r + "\n"
            "  " + c_opt + "--start" + r + "                        Démarre le conteneur après création\n"
            "\n"

            + c_sect + "Exemples :" + r + "\n"
            "  " + c_cmt + "# Créer un conteneur Ubuntu avec nginx, un utilisateur et le démarrer" + r + "\n"
            "  " + c_ex + processName + " create mycontainer --os=ubuntu \\\n"
            "      --cmd=\"apt update && apt install -y nginx\" \\\n"
            "      --env=APP_ENV=production \\\n"
            "      --user=\"alice:password:/bin/bash\" \\\n"
            "      --services=\"nginx\" --start" + r + "\n"
            "\n"
            "  " + c_cmt + "# Utiliser un rootfs personnalisé" + r + "\n"
            "  " + c_ex + processName + " create mycontainer --rootfs=./myrootfs --shell=/bin/sh --env=APP_ENV=production" + r + "\n"
            "\n"
            "  " + c_cmt + "# Depuis un fichier de configuration" + r + "\n"
            "  " + c_ex + processName + " create mycontainer --config=./mycontainer.json --start" + r + "\n"
            "\n"
            "  " + c_cmt + "# Générer un fichier de config sans créer le conteneur" + r + "\n"
            "  " + c_ex + processName + " create mycontainer --os=ubuntu --generate-config > mycontainer.json" + r + "\n"
        );
    else
    logger.raw(
        c_title + "Usage:" + r + "\n"
        "  " + colors::bold + processName + r + " " + c_opt + "create" + r + " " + c_arg + "<container_name>" + r + " [options]\n"
        "\n"
        "  " + c_cmt + "Options marked with " + c_star + "*" + c_cmt + " can be repeated to specify multiple values.\n"
        "  e.g.: --cmd=\"apt update\" --cmd=\"apt install -y curl\"" + r + "\n"
        "\n"

        + c_sect + "OS source:" + r + "\n"
        "  " + c_opt + "--os" + r + "=<name>                     Base image to use                 " + c_cmt + "(e.g.: ubuntu)" + r + "\n"
        "  " + c_opt + "--rootfs" + r + "=<path>                 Custom root filesystem            " + c_cmt + "(overrides --os)" + r + "\n"
        "  " + c_opt + "--dest" + r + "=<path>                   Container files destination       " + c_cmt + "(default: ./rootfs-os, with --os)" + r + "\n"
        "\n"

        + c_sect + "Container configuration:" + r + "\n"
        "  " + c_opt + "--shell" + r + "=<path>                  Shell to use inside container     " + c_cmt + "(default: /bin/bash)" + r + "\n"
        "  " + c_opt + "--cmd" + r + "=<command>          " + c_star + "*" + r + "      Commands run at install time      " + c_cmt + "(e.g.: \"apt update && apt install -y curl\")" + r + "\n"
        "  " + c_opt + "--env" + r + "=<KEY=VALUE>        " + c_star + "*" + r + "      Environment variables\n"
        "  " + c_opt + "--user" + r + "=<user:pass:shell> " + c_star + "*" + r + "      User to create inside container   " + c_cmt + "(e.g.: \"alice:password:/bin/bash\")" + r + "\n"
        "  " + c_opt + "--services" + r + "=<cmd>         " + c_star + "*" + r + "      Services started at boot          " + c_cmt + "(e.g.: \"nginx\")" + r + "\n"
        "\n"

        + c_sect + "Network:" + r + "\n"
        "  " + c_opt + "--host-ip" + r + "=<ip>                 Host veth IP                       " + c_cmt + "(default: 10.88.0.1)" + r + "\n"
        "  " + c_opt + "--cont-ip" + r + "=<ip>                 Container IP                       " + c_cmt + "(default: 10.88.0.2)" + r + "\n"
        "  " + c_opt + "--subnet" + r + "=<cidr>                CIDR subnet                        " + c_cmt + "(default: 10.88.0.0/24)" + r + "\n"
        "  " + c_opt + "--dns" + r + "=<ip>               " + c_star + "*" + r + "     DNS servers                        " + c_cmt + "(e.g.: \"8.8.8.8\")" + r + "\n"
        "\n"

        + c_sect + "Other:" + r + "\n"
        "  " + c_opt + "--help" + r + "                         Show this help\n"
        "  " + c_opt + "--config" + r + "=<path>                JSON config file                   " + c_cmt + "(combinable, CLI flags take precedence)" + r + "\n"
        "  " + c_opt + "--generate-config" + r + "              Generate a base JSON config file   " + c_cmt + "(does not create the container)" + r + "\n"
        "  " + c_opt + "--start" + r + "                        Start the container after creation\n"
        "\n"

        + c_sect + "Examples:" + r + "\n"
        "  " + c_cmt + "# Create an Ubuntu container with nginx, a user, and start it" + r + "\n"
        "  " + c_ex + processName + " create mycontainer --os=ubuntu \\\n"
        "      --cmd=\"apt update && apt install -y nginx\" \\\n"
        "      --env=APP_ENV=production \\\n"
        "      --user=\"alice:password:/bin/bash\" \\\n"
        "      --services=\"nginx\" --start" + r + "\n"
        "\n"
        "  " + c_cmt + "# Use a custom rootfs" + r + "\n"
        "  " + c_ex + processName + " create mycontainer --rootfs=./myrootfs --shell=/bin/sh --env=APP_ENV=production" + r + "\n"
        "\n"
        "  " + c_cmt + "# From a config file" + r + "\n"
        "  " + c_ex + processName + " create mycontainer --config=./mycontainer.json --start" + r + "\n"
        "\n"
        "  " + c_cmt + "# Generate a config file without creating the container" + r + "\n"
        "  " + c_ex + processName + " create mycontainer --os=ubuntu --generate-config > mycontainer.json" + r + "\n"
    );
}


#!/usr/bin/env bash

# ── Configuration ─────────────────────────────────────────────────────────────
CXX=g++
STD=c++17
FLAGS="-std=${STD} -Wall -Wextra"
TARGET="main"
OBJ_DIR=".obj"

# ── Couleurs ───────────────────────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
RESET='\033[0m'

ok()   { echo -e "  ${GREEN}✔${RESET}  $*"; }
fail() { echo -e "  ${RED}✘${RESET}  $*"; exit 1; }
info() { echo -e "  ${CYAN}→${RESET}  $*"; }
warn() { echo -e "  ${YELLOW}!${RESET}  $*"; }

# ── Scan des sources ───────────────────────────────────────────────────────────
mapfile -t SOURCES < <(find . \
    -path "./${OBJ_DIR}" -prune -o \
    -name "*.cpp" -print \
    | sed 's|^\./||' | sort)

[[ ${#SOURCES[@]} -eq 0 ]] && fail "Aucun fichier .cpp trouvé."

mkdir -p "${OBJ_DIR}"

# ── Affichage du scan ──────────────────────────────────────────────────────────
echo ""
echo -e "${CYAN}══ Scan ════════════════════════════════════════════${RESET}"
for src in "${SOURCES[@]}"; do info "${src}"; done
echo -e "${CYAN}══ Build (${STD}) ══════════════════════════════════════${RESET}"

# ── Compilation avec dépendances fines (-MMD -MP) ─────────────────────────────
# -MMD : génère un fichier .d à côté du .o listant les headers inclus
# -MP  : ajoute des règles fantômes pour éviter les erreurs si un .hpp est supprimé
OBJECTS=()
NEED_LINK=false

for src in "${SOURCES[@]}"; do
    obj="${OBJ_DIR}/$(echo "${src}" | tr '/' '_' | sed 's/\.cpp$/.o/')"
    dep="${obj%.o}.d"
    OBJECTS+=("${obj}")

    REBUILD=false

    # Pas d'objet → première compilation
    [[ ! -f "${obj}" ]] && REBUILD=true

    # .cpp plus récent que .o
    if [[ -f "${obj}" ]]; then
        SRC_TS=$(stat -c '%Y' "${src}")
        OBJ_TS=$(stat -c '%Y' "${obj}")
        (( SRC_TS > OBJ_TS )) && REBUILD=true
    fi

    # Lecture du fichier .d : vérifier si un header inclus est plus récent que le .o
    if [[ "${REBUILD}" == false && -f "${dep}" ]]; then
        # Le .d liste les dépendances sous la forme :
        #   obj: src header1 header2 ...
        # On extrait chaque fichier et on compare son timestamp au .o
        OBJ_TS=$(stat -c '%Y' "${obj}")
        while IFS= read -r dep_file; do
            [[ -z "${dep_file}" || ! -f "${dep_file}" ]] && continue
            DEP_TS=$(stat -c '%Y' "${dep_file}")
            if (( DEP_TS > OBJ_TS )); then
                info "Header modifié : ${dep_file}  →  recompile ${src}"
                REBUILD=true
                break
            fi
        done < <(grep -oP '[\w./\-]+\.hpp' "${dep}" 2>/dev/null)
    fi

    if [[ "${REBUILD}" == true ]]; then
        info "Compiling  ${src}"
        ${CXX} ${FLAGS} -MMD -MP -c "${src}" -o "${obj}" || fail "Échec : ${src}"
        ok "${obj}  (+ ${dep})"
        NEED_LINK=true
    else
        warn "Up-to-date ${src}"
    fi
done

# ── Link ───────────────────────────────────────────────────────────────────────
echo ""
if [[ "${NEED_LINK}" == true || ! -f "${TARGET}" ]]; then
    info "Linking    → ${TARGET}"
    ${CXX} ${FLAGS} "${OBJECTS[@]}" -o "${TARGET}" || fail "Échec du link"
    ok "Build OK  →  ./${TARGET}"
else
    ok "Rien à recompiler. Binaire à jour : ./${TARGET}"
fi

echo -e "${CYAN}═══════════════════════════════════════════════════${RESET}"
echo ""
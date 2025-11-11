#!/bin/bash

MINISHELL="./minishell"
LOG_DIR="test_logs"
mkdir -p "$LOG_DIR"

GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
NC="\033[0m"

# Vérifie si un texte attendu apparaît dans la sortie
function test_contains() {
    local cmd="$1"
    local expected="$2"
    local file="$LOG_DIR/out.txt"

    echo -e "\n${YELLOW}Test: ${cmd}${NC}"
    echo "$cmd" | $MINISHELL > "$file" 2>&1
    if grep -q "$expected" "$file"; then
        echo -e "${GREEN}[OK]${NC}"
    else
        echo -e "${RED}[KO]${NC} (n’a pas trouvé '$expected')"
        echo "---- Output ----"
        cat "$file"
        echo "----------------"
    fi
}

# Vérifie juste qu’une commande ne plante pas
function test_no_crash() {
    local cmd="$1"
    local file="$LOG_DIR/out.txt"

    echo -e "\n${YELLOW}Test: ${cmd}${NC}"
    echo "$cmd" | $MINISHELL > "$file" 2>&1
    if grep -q "Segmentation" "$file" || grep -q "abort" "$file"; then
        echo -e "${RED}[KO] Crash détecté${NC}"
        cat "$file"
    else
        echo -e "${GREEN}[OK] Aucun crash${NC}"
    fi
}

echo "==== Tests fonctionnels Minishell ===="

# --- Builtins ---
test_contains "echo hello" "hello"
test_contains "echo -n test" "test"
test_contains "pwd" "/"
test_contains "env" "="
test_contains "export VAR=42 && echo \$VAR" "42"
test_contains "unset VAR && echo \$VAR" ""
test_contains "cd / && pwd" "/"
test_contains "cd dossier_inexistant" "No such file or directory"

# --- Redirections ---
test_no_crash "echo coucou > $LOG_DIR/a.txt"
test_contains "cat $LOG_DIR/a.txt" "coucou"
test_no_crash "echo ajout >> $LOG_DIR/a.txt"
test_contains "cat $LOG_DIR/a.txt" "ajout"

# --- Pipes ---
test_contains "echo test | cat" "test"
test_contains "echo bonjour | grep bon" "bonjour"
test_contains "echo 123 | wc -c" "4"

# --- Quotes ---
test_contains "echo 'salut les amis'" "salut les amis"
test_contains "echo \"coucou \$USER\"" "coucou"

# --- Variables ---
test_contains "export X=yo && echo \$X" "yo"
test_contains "unset X && echo \$X" ""

# --- Heredoc (si supporté) ---
echo -e "ligne1\nligne2\nEOF" | $MINISHELL "cat << EOF" > "$LOG_DIR/heredoc.txt"
if grep -q "ligne1" "$LOG_DIR/heredoc.txt"; then
    echo -e "${GREEN}[OK] heredoc${NC}"
else
    echo -e "${RED}[KO] heredoc${NC}"
fi

# --- Erreurs générales ---
test_contains "ls /dossier_inexistant" "No such file or directory"
test_contains "cat fichier_inexistant" "No such file or directory"
test_contains "commandnotfound" "not found"

echo -e "\n==== Fin des tests ===="

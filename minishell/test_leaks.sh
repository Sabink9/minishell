#!/bin/bash

# ==========================================
# 🔍 GLOBAL MINISHELL LEAK TESTER (42 Style)
# ==========================================
MINISHELL=./minishell
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all \
          --track-fds=yes --trace-children=yes \
          --suppressions=readline.supp --quiet --error-exitcode=42"
LOG=leaks_report.log

# ==========================
# 🧹 Préparation
# ==========================
rm -f $LOG
touch $LOG
echo "====================================" | tee -a $LOG
echo "🧪  GLOBAL MINISHELL LEAK TESTER" | tee -a $LOG
echo "====================================" | tee -a $LOG
echo "" | tee -a $LOG

# ==========================
# 🔸 Liste de tests
# ==========================
TESTS=(
    # --- Basique ---
    "echo salut"
    "echo -n salut"
    "echo -n -n 42"
    "echo \"bonjour 42\""
    "echo \$USER"
    "echo \$?"
    "echo ''"
    # --- Variables d'environnement ---
    "env"
    "export TEST=oui && echo \$TEST"
    "unset TEST && echo \$TEST"
    # --- Redirections simples ---
    "echo coucou > file.txt"
    "cat < file.txt"
    "echo encore >> file.txt"
    "cat file.txt"
    "rm file.txt"
    # --- Pipes ---
    "echo salut | cat"
    "echo salut | grep sal"
    "ls | wc -l"
    "ls | cat | cat | wc -l"
    # --- Commandes inexistantes ---
    "qsdfqsdf"
    "catttttt"
    "|"
    "\"     \""
    # --- Builtins ---
    "pwd"
    "cd .. && pwd"
    "cd -"
    "export TEST2=42"
    "unset TEST2"
    # --- Exit ---
    "exit"
)

# ==========================
# 🧠 Exécution des tests
# ==========================
for TEST in "${TESTS[@]}"; do
    echo "------------------------------------" | tee -a $LOG
    echo "💬 Test: $TEST" | tee -a $LOG
    echo "------------------------------------" | tee -a $LOG

    printf "$TEST\nexit\n" | $VALGRIND $MINISHELL >> $LOG 2>&1
    CODE=$?

    if [ $CODE -eq 42 ]; then
        echo "❌ Leak détecté ! ($TEST)" | tee -a $LOG
    else
        echo "✅ Aucun leak détecté" | tee -a $LOG
    fi
    echo "" | tee -a $LOG
done

echo "====================================" | tee -a $LOG
echo "🏁 Tests terminés – Résumé dans $LOG"
echo "===================================="

#!/usr/bin/env bash
# test_leaks.sh
# Lance une série de tests sous valgrind pour détecter fuites mémoire sur minishell
# Usage: ./test_leaks.sh /path/to/minishell

set -euo pipefail

MINISHELL_BIN="${1:-}"
VALGRIND="$(command -v valgrind || true)"
if [[ -z "$MINISHELL_BIN" || ! -x "$MINISHELL_BIN" ]]; then
  echo "Usage: $0 /chemin/vers/minishell_executable"
  exit 1
fi
if [[ -z "$VALGRIND" ]]; then
  echo "valgrind n'est pas installé ou introuvable dans PATH."
  exit 1
fi

OUTDIR="valgrind_tests_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$OUTDIR"
echo "Logs dans $OUTDIR"

VG_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --num-callers=30 --errors-for-leak-kinds=all"

# helper: run minishell with a here-doc of commands under valgrind
run_vg_here() {
  local name="$1"; shift
  local cmds="$*"
  local logfile="${OUTDIR}/${name}.vg.log"
  echo "=== RUN $name ==="
  echo "$cmds" | valgrind $VG_OPTS --log-file="$logfile" "$MINISHELL_BIN" >/dev/null 2>&1 || true
  # extract summary
  echo "---- $name summary ----" > "${OUTDIR}/${name}.summary.txt"
  grep -E "definitely lost:|indirectly lost:|still reachable:|ERROR SUMMARY" "$logfile" >> "${OUTDIR}/${name}.summary.txt" || true
  tail -n 200 "$logfile" >> "${OUTDIR}/${name}.summary.txt" || true
}

# helper: quick parse of summaries into global report
make_report() {
  echo "==== GLOBAL REPORT ====" > "${OUTDIR}/global_report.txt"
  for f in "${OUTDIR}"/*.summary.txt; do
    echo "---- $(basename "$f" .summary.txt) ----" >> "${OUTDIR}/global_report.txt"
    grep -E "definitely lost:|indirectly lost:|still reachable:|ERROR SUMMARY" "$f" >> "${OUTDIR}/global_report.txt" || true
    echo "" >> "${OUTDIR}/global_report.txt"
  done
  echo "Report ready: ${OUTDIR}/global_report.txt"
}

# Prepare a big environment (stress test of env duplication/manipulation)
generate_big_envfile() {
  local file="$1"
  : > "$file"
  for i in $(seq 1 1000); do
    printf "VAR_%04d=valeur_longue_%04d\n" "$i" "$i" >> "$file"
  done
}

# 1) tests simples / builtins
run_vg_here "builtin_echo_pwd_env" $'echo hello\npwd\nenv | head -n 3\nexit\n'

# 2) export / unset many times (stress)
run_vg_here "export_unset_small" $'export A=1\nexport B=2\nunset A\nunset B\nexit\n'

run_vg_here "export_unset_loop" $'export LOOP0=0\n'"$(for i in $(seq 1 200); do echo "export X$i=val$i"; done)"$'\n'"$(for i in $(seq 1 200); do echo "unset X$i"; done)"$'\nexit\n'

# 3) pipeline + external programs (execve paths)
run_vg_here "pipes_external" $'export TEST=hello\necho $TEST | grep TEST || true\nls -la | grep -i bin\nexit\n'

# 4) redirections > >> < and combine with pipes
run_vg_here "redirs_and_pipes" $'echo toto > /tmp/ms_test1.txt\ncat < /tmp/ms_test1.txt | grep toto > /tmp/ms_test2.txt\necho done >> /tmp/ms_test2.txt\nexit\n'

# 5) heredoc simple and heredoc in pipeline
run_vg_here "heredoc_simple" $'cat <<EOF\nline1\nline2\nEOF\nexit\n'

run_vg_here "heredoc_pipe" $'cat <<EOF | grep line\nlineA\nlineB\nEOF\nexit\n'

# 6) quotes and continuation (unclosed quotes)
run_vg_here "unclosed_quotes" $'echo "start\nmiddle\nend"\nexit\n'

# 7) variable expansion: many envs / long names
TMPENV="${OUTDIR}/bigenv.txt"
generate_big_envfile "$TMPENV"
# create a wrapper that sources env into a temp envp: we will set env for valgrind by prefixing
# Valgrind inherits environment; here we just run minishell with env exported.
(
  set -a
  # load big env into shell env for child
  while IFS= read -r line; do export "$line"; done < "$TMPENV"
  # run a single valgrind test where minishell will copy env etc.
  echo 'echo $VAR_0001\necho $VAR_0500\nexit' | valgrind $VG_OPTS --log-file="${OUTDIR}/big_env.vg.log" "$MINISHELL_BIN" >/dev/null 2>&1 || true
)
grep -E "definitely lost:|indirectly lost:|still reachable:|ERROR SUMMARY" "${OUTDIR}/big_env.vg.log" > "${OUTDIR}/big_env.summary.txt" || true

# 8) expansion edgecases: $?, $NOTEXIST, mixed with quotes
run_vg_here "expansions_edgecases" $'false\necho $?\necho $NOT_EXIST\nexport FOO=bar\necho "$FOO" \necho '\''$FOO'\''\nexit\n'

# 9) long line / huge token
long_line=$(python3 - <<'PY'
print("echo " + ("A"*10000))
PY
)
run_vg_here "long_line" "$long_line"$'\nexit\n'

# 10) repeated runs to catch cumulative leaks (loop many commands)
cat > "${OUTDIR}/loop_cmds.txt" <<'EOF'
for i in $(seq 1 200); do
  echo "loop $i"
  export L$i=val$i
  unset L$i
done
exit
EOF
run_vg_here "loop_many" "$(cat ${OUTDIR}/loop_cmds.txt)"

# 11) test with empty PATH (should still handle absolute/relative commands correctly)
run_vg_here "empty_path" $'export PATH=\n/bin/ls\nexit\n'

# 12) simulate SIGINT during a long-running builtin/external (tries to reproduce "Ctrl-C then next cmd fails")
# We launch minishell, send commands with sleep, then send SIGINT and continue.
sigtest_log="${OUTDIR}/sigint_test.vg.log"
(
  # open minishell under valgrind in background with fifo as input
  mkfifo "${OUTDIR}/fifo_sig"
  # feed commands slowly in background, main process will run under valgrind
  (
    # first command that will be interrupted
    printf "cat > /dev/null\n" > "${OUTDIR}/fifo_sig"
    sleep 0.1
    # send SIGINT to valgrind+minishell process after it's started
  ) &

  # launch valgrind + minishell, we'll send SIGINT to its PID from this subshell
  valgrind $VG_OPTS --log-file="$sigtest_log" "$MINISHELL_BIN" < "${OUTDIR}/fifo_sig" &
  VG_PID=$!
  # wait a bit to let it start, then send SIGINT to the process group of the child
  sleep 0.3
  kill -SIGINT $VG_PID || true
  # then feed more commands to see if prompt/next commands work
  sleep 0.1
  printf "echo aftersig\nexit\n" > "${OUTDIR}/fifo_sig"
  wait $VG_PID || true
  rm -f "${OUTDIR}/fifo_sig"
)
grep -E "definitely lost:|indirectly lost:|still reachable:|ERROR SUMMARY" "$sigtest_log" > "${OUTDIR}/sigint_test.summary.txt" || true

# 13) test behavior when export returns new env (call flows that copy/free env)
run_vg_here "export_return_env_changes" $'export TEST_EXPORT=on\nenv | grep TEST_EXPORT\nunset TEST_EXPORT\nenv | grep TEST_EXPORT || true\nexit\n'

# Summarize
make_report

echo "Toutes les commandes ont été lancées. Les logs valgrind sont dans $OUTDIR"
echo "Consulte ${OUTDIR}/global_report.txt pour le résumé. Pour voir un log complet: less ${OUTDIR}/<test>.vg.log"

exit 0

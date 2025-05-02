#!/bin/bash

# Commande à tester
CMD='/bin/echo -n test1		test2'

# Exécution avec minishell
echo "$CMD" | ./minishell > out_mini.txt 2> err_mini.txt

# Exécution avec bash
echo "$CMD" | bash > out_bash.txt 2> err_bash.txt

# Affichage des résultats
echo "=== STDOUT DIFF ==="
diff -u out_bash.txt out_mini.txt || echo "[❌] Différences sur STDOUT"

echo
echo "=== STDERR DIFF ==="
diff -u err_bash.txt err_mini.txt || echo "[❌] Différences sur STDERR"

echo
echo "=== RAW OUTPUT ==="
echo "--- minishell STDOUT ---"
cat -A out_mini.txt
echo "--- bash STDOUT ---"
cat -A out_bash.txt

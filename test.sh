#!/usr/bin/env bash
# =======================================
#  COMPLETE PHILO TEST SCRIPT  – v2
# =======================================
# - Each test line ends with “:seconds” → its own timeout
# - Any non-zero exit code is still counted as success
#   for the cases that are *supposed* to error-out.

BIN=./philo
LOGFILE=philo_test_full.log
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

print_result () {
    if [ "$1" -eq 0 ]; then
        echo -e "${GREEN}[OK]${RESET} $2"
    else
        echo -e "${RED}[FAIL]${RESET} $2"
    fi
}

# -------------------------------------------------
# 1. Rebuild
# -------------------------------------------------
echo    "=======================================" > "$LOGFILE"
echo    "Starting full philo project test suite" >> "$LOGFILE"
echo    "=======================================" >> "$LOGFILE"
echo -e "${GREEN}Rebuilding …${RESET}"
make -s re                                >> "$LOGFILE" 2>&1 || {
    echo -e "${RED}Build failed – see log${RESET}"
    exit 1
}

# -------------------------------------------------
# 2. Functional & edge tests
#    Format:  "arg1 arg2 …:timeout_in_seconds"
# -------------------------------------------------
tests=(
  "20000000000 800 200 200:3"        # overflow → immediate return
  "1 500 200 200:3"                  # single philo dies
  "4 800 200 200:15"                 # steady state
  "4 800 200 200 3:15"               # stops after 3 meals
  "2 310 200 200:4"                  # death at 311 ms
  "2 800 200 200:15"                 # pair steady
  "5 800 0 200 5:3"                  # invalid arg → error
  "1 1 1 1:3"                        # ultra-fast death
  "5 10000 10000 10000 3:80"         # very long run, needs ≥70 s
)

echo -e "\n${GREEN}--- Functional tests ---${RESET}"
for entry in "${tests[@]}"; do
    args="${entry%%:*}"
    tmo="${entry##*:}"

    echo "--------------------------------------"      | tee -a "$LOGFILE"
    echo "Running: ./philo $args (timeout ${tmo}s)"    | tee -a "$LOGFILE"

    if timeout "${tmo}"s $BIN $args                    >> "$LOGFILE" 2>&1; then
        print_result 0 "$args"
    else
        code=$?
        if [ $code -eq 124 ]; then
            print_result 1 "Test timed-out: $args"
        else
            # Non-zero exit code is expected for the error-input cases
            if [[ "$args" =~ 20000000000|800\ 0 ]]; then
                print_result 0 "$args (expected error)"
            else
                print_result 1 "Exited with code $code: $args"
            fi
        fi
    fi
done

# -------------------------------------------------
# 3. Valgrind leak test  (15 s is plenty)
# -------------------------------------------------
echo -e "\n${GREEN}--- Valgrind leak test ---${RESET}"
echo "======================================="                 >> "$LOGFILE"
echo "Running Valgrind on: ./philo 5 800 200 200 3"           >> "$LOGFILE"

if timeout 15s valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes $BIN 5 800 200 200 3               \
         >> "$LOGFILE" 2>&1; then
    print_result 0 "Valgrind completed"
else
    print_result 1 "Valgrind timed-out or errored"
fi

echo -e "\n${GREEN}✅  All tests done.  See '${LOGFILE}' for details.${RESET}"

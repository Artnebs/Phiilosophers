#!/bin/bash

# =======================================
#  COMPLETE PHILO TEST SCRIPT
# =======================================
# Includes:
# - Argument edge cases
# - Functional cases (starvation, normal execution)
# - Valgrind memory leak check
# - Log outputs

BIN="./philo"
LOGFILE="philo_test_full.log"
TIMEOUT_DURATION=10
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

print_result() {
	if [ "$1" -eq 0 ]; then
		echo -e "${GREEN}[OK]${RESET} $2"
	else
		echo -e "${RED}[FAIL]${RESET} $2"
	fi
}

# Reset log file
echo "======================================" > "$LOGFILE"
echo "Starting full philo project test suite" >> "$LOGFILE"
echo "======================================" >> "$LOGFILE"

echo "Rebuilding..." | tee -a "$LOGFILE"
make re >> "$LOGFILE" 2>&1
if [ ! -f "$BIN" ]; then
	echo "Error: philo binary not found." | tee -a "$LOGFILE"
	exit 1
fi

# --------------------------
# Functional & edge tests
# --------------------------
declare -a tests=(
	"20000000000 800 200 200"     # Overflow test
	"1 500 200 200"               # One philosopher - expected to die
	"4 800 200 200"               # Normal case - no one dies
	"4 800 200 200 3"             # Should stop after 3 meals per philosopher
	"2 310 200 200"               # No print after death
	"2 800 200 200"               # Philosopher pairs
	"5 800 0 200 5"               # time_to_eat = 0 (busy edge case)
	"1 1 1 1"                     # Extremely fast starvation
	"5 10000 10000 10000 3"       # Very high times
)

for test in "${tests[@]}"; do
	echo "--------------------------------------" | tee -a "$LOGFILE"
	echo "Running: ./philo $test" | tee -a "$LOGFILE"
	timeout ${TIMEOUT_DURATION}s $BIN $test >> "$LOGFILE" 2>&1
	code=$?
	if [ $code -eq 124 ]; then
		print_result 1 "Test timed out: $test"
	else
		print_result 0 "Test finished: $test"
	fi
done

# --------------------------
# Valgrind memory test
# --------------------------
echo "======================================" >> "$LOGFILE"
echo "Running Valgrind on: ./philo 5 800 200 200 3" | tee -a "$LOGFILE"
timeout ${TIMEOUT_DURATION}s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $BIN 5 800 200 200 3 >> "$LOGFILE" 2>&1
if [ $? -eq 124 ]; then
	print_result 1 "Valgrind timed out"
else
	print_result 0 "Valgrind completed"
fi

echo "======================================" >> "$LOGFILE"
echo "✅ All tests done. See $LOGFILE for full logs."
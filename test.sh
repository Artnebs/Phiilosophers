#!/bin/bash
# test_with_log.sh - Script to run tests for the philo project and save all output to a log file.
#
# This script will:
# 1. Rebuild the project using the Makefile.
# 2. Run several tests with different parameters.
# 3. Use Valgrind to check for memory leaks.
# 4. Save all output (logs) to a file named philo_test.log.
#
# Usage: ./test_with_log.sh
# After running, you can open philo_test.log and feed its contents for further analysis.

LOGFILE="philo_test.log"

# Clear previous log contents.
echo "======================================" > "$LOGFILE"
echo "Starting tests for philo project" >> "$LOGFILE"
echo "======================================" >> "$LOGFILE"

# ---------------------------------------------------------------------------- #
# Rebuild the project
# ---------------------------------------------------------------------------- #
echo "Rebuilding the project..." | tee -a "$LOGFILE"
make re >> "$LOGFILE" 2>&1

# Check if the binary exists.
if [ ! -f "./philo" ]; then
    echo "Error: 'philo' binary not found!" | tee -a "$LOGFILE"
    exit 1
fi

# ---------------------------------------------------------------------------- #
# Define tests to run
# ---------------------------------------------------------------------------- #
# Add or modify test cases here.
tests=(
    "1 800 200 200"        # Single philosopher; expected to eventually starve.
    "5 800 200 200 7"      # 5 philosophers; simulation stops after each eats 7 times.
    "4 310 200 100"        # Test with tight timing; a philosopher should die quickly.
)

# ---------------------------------------------------------------------------- #
# Run each test and log the output
# ---------------------------------------------------------------------------- #
echo "======================================" >> "$LOGFILE"
echo "Running simulation tests..." | tee -a "$LOGFILE"

for test in "${tests[@]}"; do
    echo "--------------------------------------" >> "$LOGFILE"
    echo "Running: ./philo $test" | tee -a "$LOGFILE"
    # Run the simulation and append its output to the log file.
    ./philo $test >> "$LOGFILE" 2>&1
    echo "Test finished." | tee -a "$LOGFILE"
done

# ---------------------------------------------------------------------------- #
# Run Valgrind to check for memory leaks on one test case
# ---------------------------------------------------------------------------- #
echo "======================================" >> "$LOGFILE"
echo "Running Valgrind for memory leak check on: ./philo 5 800 200 200 7" | tee -a "$LOGFILE"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo 5 800 200 200 7 >> "$LOGFILE" 2>&1

echo "======================================" >> "$LOGFILE"
echo "All tests completed. Log saved to $LOGFILE" | tee -a "$LOGFILE"

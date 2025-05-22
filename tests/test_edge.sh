#!/bin/bash
# test_edge.sh - Script to run edge-case tests on the philo project.
#
# This script will:
#   1. Rebuild the project.
#   2. Run a series of edge tests with extreme or unusual parameters.
#   3. Use a timeout to prevent any test from hanging indefinitely.
#   4. Run Valgrind on one test case to check for memory leaks.
#   5. Log all output (simulation output and error messages) to philo_edge_test.log.
#
# Usage:
#   chmod +x test_edge.sh
#   ./test_edge.sh
#
# You can then review the contents of philo_edge_test.log.

LOGFILE="philo_edge_test.log"
TIMEOUT_DURATION=10  # Timeout (in seconds) for each test

# Clear previous log contents.
echo "======================================" > "$LOGFILE"
echo "Starting edge tests for philo project" >> "$LOGFILE"
echo "======================================" >> "$LOGFILE"

# ---------------------------------------------------------------------------- #
# Rebuild the project.
# ---------------------------------------------------------------------------- #
echo "Rebuilding the project..." | tee -a "$LOGFILE"
make re >> "$LOGFILE" 2>&1

# Verify that the philo binary exists.
if [ ! -f "./philo" ]; then
    echo "Error: 'philo' binary not found!" | tee -a "$LOGFILE"
    exit 1
fi

# ---------------------------------------------------------------------------- #
# Define edge-case tests.
# ---------------------------------------------------------------------------- #
# These tests include:
#   - Minimal parameters (extremely short times, causing an almost immediate death).
#   - A meal count of 0 (should cause the simulation to end immediately).
#   - A case where time_to_eat is 0 (which might create a busy loop).
#   - A large number of philosophers to stress the thread handling.
#   - Very high time values (resulting in a very slow simulation).
declare -a tests=(
    "1 1 1 1"                     # Minimal parameters: a single philosopher with all times set to 1ms.
    "5 800 200 200 0"             # Meal count of 0: simulation should immediately stop.
    "5 800 0 200 7"               # time_to_eat is 0: may lead to a busy loop or unexpected behavior.
    "200 800 200 200"             # Many philosophers: stresses thread management.
    "5 10000 10000 10000 3"        # Very high times: simulation runs very slowly.
)

# ---------------------------------------------------------------------------- #
# Run each edge-case test with a timeout and log the output.
# ---------------------------------------------------------------------------- #
echo "======================================" >> "$LOGFILE"
echo "Running edge-case tests (timeout: ${TIMEOUT_DURATION}s each)..." | tee -a "$LOGFILE"

for test in "${tests[@]}"; do
    echo "--------------------------------------" >> "$LOGFILE"
    echo "Edge test: ./philo $test" | tee -a "$LOGFILE"
    
    # Run the simulation with a timeout. If it doesn't finish in TIMEOUT_DURATION seconds, it is killed.
    timeout ${TIMEOUT_DURATION}s ./philo $test >> "$LOGFILE" 2>&1
    ret_val=$?
    
    if [ $ret_val -eq 124 ]; then
         echo "Test timed out after ${TIMEOUT_DURATION}s." | tee -a "$LOGFILE"
    else
         echo "Test finished with exit code $ret_val." | tee -a "$LOGFILE"
    fi
done

# ---------------------------------------------------------------------------- #
# Run Valgrind on one edge test to check for memory leaks.
# ---------------------------------------------------------------------------- #
echo "======================================" >> "$LOGFILE"
echo "Running Valgrind for memory leak check on edge test: ./philo 5 800 200 200 0" | tee -a "$LOGFILE"

timeout ${TIMEOUT_DURATION}s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo 5 800 200 200 0 >> "$LOGFILE" 2>&1
valgrind_ret=$?
if [ $valgrind_ret -eq 124 ]; then
    echo "Valgrind test timed out after ${TIMEOUT_DURATION}s." | tee -a "$LOGFILE"
else
    echo "Valgrind test finished with exit code $valgrind_ret." | tee -a "$LOGFILE"
fi

echo "======================================" >> "$LOGFILE"
echo "Edge tests completed. Log saved to $LOGFILE" | tee -a "$LOGFILE"

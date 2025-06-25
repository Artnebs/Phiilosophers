#!/usr/bin/env bash
# philo full test – extended v3

BIN=./philo
LOG=philo_test_full.log
GREEN="\033[0;32m" RED="\033[0;31m" RESET="\033[0m"

print_ok()    { echo -e "${GREEN}[ OK ]${RESET} $1"; }
print_fail()  { echo -e "${RED}[FAIL]${RESET} $1"; }

# Rebuild
echo "=== philo test suite ===" > "$LOG"
make re &>>"$LOG" || { print_fail "build"; exit 1; }

# test definitions: "args:timeout:should_error?"
tests=(
  "20000000000 800 200 200:3:yes"    # overflow
  "0 800 200 200:1:yes"              # zero philosophers
  "5 800 200 0:1:yes"                # zero time_to_sleep
  "5 800 0 200:1:yes"                # zero time_to_eat
  "1 500 200 200:3:no"               # 1 philo dies
  "4 800 200 200:15:no"              # steady state
  "4 800 200 200 3:15:no"            # stops after 3 meals
  "2 310 200 200:4:no"               # death at ~310ms
  "2 800 200 200:15:no"              # 2‐philo steady
  "1 1 1 1:3:no"                     # ultra‐fast death
  "5 10000 10000 10000 3:80:no"      # very long run
  "5 800 200:1:yes"                  # missing arg
  "5 800 ab 200:1:yes"               # non‐numeric
)

echo; echo -e "${GREEN}-- Functional tests --${RESET}"
for entry in "${tests[@]}"; do
  IFS=":" read -r args tmo should_err <<<"$entry"
  echo "→ $BIN $args (timeout ${tmo}s)" | tee -a "$LOG"
  
  timeout "${tmo}s" $BIN $args &>>"$LOG"
  code=$?
  
  if [ $code -eq 124 ]; then
    print_fail "timeout on: $args"
  else
    if [ "$should_err" = "yes" ]; then
      if [ $code -ne 0 ]; then
        print_ok "$args (expected error)"
      else
        print_fail "$args should have failed but exited 0"
      fi
    else
      if [ $code -eq 0 ]; then
        print_ok "$args"
      else
        print_fail "$args exited $code"
      fi
    fi
  fi
done

# Valgrind
echo; echo -e "${GREEN}-- Valgrind leak test --${RESET}"
timeout 15s valgrind --leak-check=full --error-exitcode=1 $BIN 5 800

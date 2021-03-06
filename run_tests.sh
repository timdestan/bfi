#!/bin/bash

set -o errexit
set -o nounset

BLACK=$(tput setaf 0)
BLUE=$(tput setaf 1)
GREEN=$(tput setaf 2)
CYAN=$(tput setaf 3)
RED=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
YELLOW=$(tput setaf 6)
WHITE=$(tput setaf 7)

BOLD=$(tput bold)
RESET=$(tput sgr0)

function summary() {
  echo -e "$BOLD$GREEN==>$WHITE ${1}$RESET"
}

function run_program() {
  local exe=$1
  local input=$2
  local expected=$3

  if [ -f $input ]; then
    $exe < $input > output.txt
  else
    $exe > output.txt
  fi

  # Compare output.
  if [ -f $expected ]; then
    diff output.txt $expected > /dev/null
  fi

  echo "OK"
}

function run_interpreter() {
  local test_program=$1

  # Run the interpeter, saving output.
  local exe="./bfi examples/$test_program"
  local input=examples/${test_program}.in
  local expected=examples/${test_program}.out

  run_program "$exe" $input $expected
}

function run_codegen() {
  local test_program=$1
  local input=examples/${test_program}.in
  local expected=examples/${test_program}.out

  local bfsrc="examples/$test_program"
  local csrc="${bfsrc/.bf/.c}"
  local exe="${csrc/.c/.exe}"

  ./bfgen $bfsrc > $csrc
  clang $csrc -o $exe

  run_program $exe $input $expected
}

function check_interpreter() {
  summary "Testing Interpreter: $1"
  run_interpreter $1

  # Cleanup.
  rm output.txt
}

function check_codegen() {
  summary "Testing Codegen: $1"
  run_codegen $1

  # Cleanup.
  rm output.txt
  rm examples/*.c
  rm examples/*.exe
}

function fast_tests() {
  check_interpreter bangbang.bf
  check_interpreter bottles.bf
  check_interpreter factor.bf
  check_interpreter hello_world.bf

  check_codegen bangbang.bf
  check_codegen bottles.bf
  check_codegen factor.bf
  check_codegen hello_world.bf
}

function slow_tests() {
  # Comment out most of these because it's so painful to run them.
  # interpreter tests are particularly slow.

  #  check_interpreter life.bf
  #  check_interpreter mandelbrot.bf

  check_codegen life.bf
  # check_codegen mandelbrot.bf
}

make
fast_tests
slow_tests

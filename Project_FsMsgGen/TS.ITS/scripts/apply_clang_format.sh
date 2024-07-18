#!/bin/bash

# Debug mode
set -vx
#set -e

find ../ccsrc -type f -name "*.hh" -exec clang-format -style=file -i {} \;
find ../ccsrc -type f -name "*.cc" -exec clang-format -style=file -i {} \;
find ../ccsrc -type f -name "*.h" -exec clang-format -style=file -i {} \;
find ../ccsrc -type f -name "*.c" -exec clang-format -style=file -i {} \;

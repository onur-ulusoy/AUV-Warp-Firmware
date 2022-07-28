#!/bin/bash

# Argument parsing
VERBOSE=0
for arg in "$@"
do
    if [ "$arg" == "--v" ]; then
        VERBOSE=1
    fi
done

# Cleaning previous builds
echo -e "\033[1;33m[INFO] Cleaning previous build...\033[0m"
make clean

# Building the project
echo -e "\033[1;33m[INFO] Building the project...\033[0m"
if [ $VERBOSE -eq 1 ]; then
    # If verbose flag is provided, print warnings and errors
    make all &> build.log
    MAKE_EXIT_CODE=$?
    # Print warnings and errors
    grep --color -E "warning:|error:" build.log
else
    # Otherwise, print all output
    make all
    MAKE_EXIT_CODE=$?
fi

# Check the exit status of the make command and inform the user
if [ $MAKE_EXIT_CODE -eq 0 ]; then
    echo -e "\033[1;32m[INFO] Build successful!\033[0m"
else
    echo -e "\033[1;31m[ERROR] Build failed. Check above for error details.\033[0m"
fi

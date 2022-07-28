#!/bin/bash

# Clean up previous build files
echo "Cleaning up previous build files..."
make clean

# Compile the project
echo "Compiling the project..."
make all

# If the project compiled successfully, print a success message
if [ $? -eq 0 ]; then
    echo "Project compiled successfully!"
else
    echo "Project compilation failed. Please check the error messages above."
fi
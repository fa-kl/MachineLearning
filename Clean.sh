#!/bin/bash

# Check if Build directory exists
if [ -d "Build" ]; then
    # Remove everything inside Build directory
    rm -rf Build/*
else
    echo "Error: Build directory does not exist"
    exit 1
fi
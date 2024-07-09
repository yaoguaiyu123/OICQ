#!/bin/bash

if [ -x "./appoicqclient" ]; then

    ./appoicqclient
else
    echo "Error: appoicqclient does not exist or is not executable in the current directory."
    exit 1
fi


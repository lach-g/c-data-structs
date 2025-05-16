#!/usr/bin/env bash
mkdir -p build
gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 -c lach.h -o build/liblach.o
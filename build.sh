#!/bin/bash
gcc -Wall -std=c99 -c gol.c
gcc -Wall -o other gol.o other.c

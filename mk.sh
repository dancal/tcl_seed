#!/bin/sh
swig -tcl8 swig_seed.i
gcc -fpic -c swig_seed_wrap.c
gcc -shared swig_seed_wrap.o -o libseed.so -lm

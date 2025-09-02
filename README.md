# Lisp-Compiler

This LISP compiler is a work in progress. 

## Prerequisites
* clang version 20.1.8
* LLVM version 20.1.8

## Current build steps
1. `make`
2. `./driver test.lisp`
3. `sh link.sh`
4. `./a.out`

The driver produces LLVM IR, however, a number of built-in functionality is stored in the LISP runtime. The `sh link.sh` build step will compile the LLVM IR code into a native binary, with the runtime functions included.

## Note for the curious
The only functions implemented thus far are the ones demonstrated in `test.lisp`. Currently all testing is done by examining the compiler output. 

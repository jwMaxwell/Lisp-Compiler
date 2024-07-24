# TLDR

## driver.cpp

This is the main class. It grabs the command line arguments, reads the input file, and runs the top level compilation steps.

## tokenizer.cpp / tokenizer.h

Lisp doesn't really need a lexer or and lexical analysis. So I made a tokenizer. The tokenizer splits the input into tokens containing the token value, type, and location.

## parser.cpp / parser.h

The parser parses. Go figure. This basically parses the same way as my Lisp interpreter but it has plenty of C++ ugliness.

## nodes.h

This file contains the various AST node classes. Classes are kinda necessary for constructing a tree structure and they are very handy when implementing LLVM code generation (codegen()).

## functions.cpp / functions.h

Built-in functions for Lisp will need to be defined somewhere and it would be best not to clutter nodes.h any more than it already is.

## llvm_lisp.cpp / llvm_lisp.h

The compiler will have some constants, most of which are LLVM related. These files pretty much exist just for these purposes.

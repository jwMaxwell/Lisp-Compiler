#ifndef LLVM_LISP_H
#define LLVM_LISP_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <map>
#include <string>
#include <iostream>

// This is an object that owns LLVM core data structures
extern llvm::LLVMContext the_context;

// This is a helper object that makes it easy to generate LLVM instructions
extern llvm::IRBuilder<> the_builder;

// This is an LLVM construct that contains functions and global variables
extern std::unique_ptr<llvm::Module> the_module;

// This map keeps track of which values are defined in the current scope
extern std::map<std::string, llvm::Value *> named_values;

extern void log_debug(std::string stmt);

extern void log_error(std::string error);

extern llvm::Value* log_error_v(const char* str);

extern llvm::Function* log_error_f(const char* str);

void init_llvm();

#endif // LLVM_LISP_H

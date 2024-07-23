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
extern llvm::LLVMContext TheContext;

// This is a helper object that makes it easy to generate LLVM instructions
extern llvm::IRBuilder<> Builder;

// This is an LLVM construct that contains functions and global variables
extern std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
extern std::map<std::string, llvm::Value *> NamedValues;

extern void LogDebug(std::string stmt);

extern void LogError(std::string error);

extern llvm::Value* LogErrorV(const char* str);

extern llvm::Function* LogErrorF(const char* str);

void initLLVM();

#endif // LLVM_LISP_H

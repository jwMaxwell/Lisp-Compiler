#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include "nodes.h"

llvm::Function* initAddFunction(llvm::Module* module, llvm::LLVMContext& context);
llvm::Function* initSubFunction(llvm::Module* module, llvm::LLVMContext& context);
llvm::Value* createCallFunction(llvm::Function* targetFunction, llvm::Value* arg1, llvm::Value* arg2);
void initFunctions(llvm::Module* module, llvm::LLVMContext& context);

extern llvm::Function* addFn;
extern llvm::Function* subFn;

#endif // FUNCTIONS_H

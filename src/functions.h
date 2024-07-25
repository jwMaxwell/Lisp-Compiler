#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include "nodes.h"

llvm::Function* init_add_fn(llvm::Module* module, llvm::LLVMContext& context);
llvm::Function* init_sub_fn(llvm::Module* module, llvm::LLVMContext& context);
llvm::Value* call_function(llvm::Function* target_function, llvm::Value* arg1, llvm::Value* arg2);
void init_functions(llvm::Module* module, llvm::LLVMContext& context);

extern llvm::Function* add_fn;
extern llvm::Function* sub_fn;
extern llvm::Function* mul_fn;
extern llvm::Function* div_fn;

#endif // FUNCTIONS_H

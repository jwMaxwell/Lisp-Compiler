#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

void init_functions(llvm::Module *module, llvm::LLVMContext &context);

extern llvm::Function *add_fn;
extern llvm::Function *sub_fn;
extern llvm::Function *mul_fn;
extern llvm::Function *div_fn;
extern llvm::Function *print_fn;

#endif // FUNCTIONS_H

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../llvm/llvm_lisp.h"
#include "../runtime/runtime_ir.h"
#include "Literal.h"
#include "node_fwd.h"
#include <llvm/IR/Value.h>

inline llvm::Value *add_codegen(std::vector<llvm::Value *> children) {
  log_debug("add_codegen()");

  llvm::Value *acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    llvm::Value *next = children.at(i);
    acc = the_builder.CreateCall(runtime_ir.get_add, {acc, next}, "add");
  }
  return acc;
}

inline llvm::Value *sub_codegen(std::vector<llvm::Value *> children) {
  log_debug("sub_codegen()");

  llvm::Value *acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    llvm::Value *next = children.at(i);
    acc = the_builder.CreateCall(runtime_ir.get_sub, {acc, next}, "sub");
  }
  return acc;
}

inline llvm::Value *mul_codegen(std::vector<llvm::Value *> children) {
  log_debug("mul_codegen()");

  llvm::Value *acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    llvm::Value *next = children.at(i);
    acc = the_builder.CreateCall(runtime_ir.get_mul, {acc, next}, "mul");
  }
  return acc;
}

inline llvm::Value *div_codegen(std::vector<llvm::Value *> children) {
  log_debug("div_codegen()");

  llvm::Value *acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    llvm::Value *next = children.at(i);
    acc = the_builder.CreateCall(runtime_ir.get_div, {acc, next}, "div");
  }
  return acc;
}

#endif // FUNCTIONS_H_

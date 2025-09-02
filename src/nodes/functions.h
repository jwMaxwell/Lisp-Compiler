#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "../llvm/llvm_lisp.h"
#include "../runtime/runtime_ir.h"
#include "codegen_result.h"
#include "literals/Literal.h"
#include "node_fwd.h"
#include <llvm/IR/Value.h>
#include <vector>

inline CodegenResult add_codegen(std::vector<CodegenResult> children) {
  log_debug("add_codegen()");

  CodegenResult acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    CodegenResult next = children.at(i);
    auto *temp = the_builder.CreateFAdd(acc.value, next.value, "addtmp");
    acc = {temp, temp->getType(), false};
  }
  return acc;
}

inline CodegenResult sub_codegen(std::vector<CodegenResult> children) {
  log_debug("sub_codegen()");

  CodegenResult acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    CodegenResult next = children.at(i);
    auto *temp = the_builder.CreateFSub(acc.value, next.value, "subtmp");
    acc = {temp, temp->getType(), false};
  }
  return acc;
}

inline CodegenResult mul_codegen(std::vector<CodegenResult> children) {
  log_debug("mul_codegen()");

  CodegenResult acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    CodegenResult next = children.at(i);
    auto *temp = the_builder.CreateFMul(acc.value, next.value, "multmp");
    acc = {temp, temp->getType(), false};
  }
  return acc;
}

inline CodegenResult div_codegen(std::vector<CodegenResult> children) {
  log_debug("div_codegen()");

  CodegenResult acc = children.at(0);
  for (size_t i = 1; i < children.size(); ++i) {
    CodegenResult next = children.at(i);
    auto *temp = the_builder.CreateFDiv(acc.value, next.value, "divtmp");
    acc = {temp, temp->getType(), false};
  }
  return acc;
}

inline llvm::Value *box_value(llvm::Value *v) {
  const llvm::Type *TYPE = v->getType();
  auto *i8ptr = getInt8PtrTy(the_context);

  if (TYPE->isDoubleTy()) {
    return the_builder.CreateCall(runtime_ir.box_number, {v}, "box_value");
  }

  if (TYPE->isPointerTy()) {
    return the_builder.CreateBitCast(v, i8ptr, "ptr_to_i8");
  }

  return v;
}

#endif // FUNCTIONS_H_

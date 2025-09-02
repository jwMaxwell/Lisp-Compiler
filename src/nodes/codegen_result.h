#ifndef CODEGEN_RESULT_H_
#define CODEGEN_RESULT_H_

#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"

struct CodegenResult {
  llvm::Value *value;
  llvm::Type *type;
  bool isBoxed;

  CodegenResult(llvm::Value *v = nullptr, llvm::Type *t = nullptr,
                bool boxed = true)
      : value(v), type(t), isBoxed(boxed) {}
};

#endif // CODEGEN_RESULT_H_

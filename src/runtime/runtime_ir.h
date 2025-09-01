#ifndef RUNTIME_IR_H
#define RUNTIME_IR_H
#include <llvm/IR/Function.h>

struct RuntimeIR {
  llvm::Function *nil_handle = nullptr;
  llvm::Function *bool_handle = nullptr;
  llvm::Function *number_handle = nullptr;
  llvm::Function *string_handle = nullptr;
  llvm::Function *symbol_handle = nullptr;
  llvm::Function *cons = nullptr;
  llvm::Function *car = nullptr;
  llvm::Function *cdr = nullptr;
  llvm::Function *is_atom = nullptr; // returns i32
  llvm::Function *eq = nullptr;      // returns i32
  llvm::Function *get_add = nullptr;
  llvm::Function *get_sub = nullptr;
  llvm::Function *get_mul = nullptr;
  llvm::Function *get_div = nullptr;
  llvm::Function *print_value = nullptr; // void(Value*)

  llvm::Function *gc_root_push = nullptr;
  llvm::Function *gc_root_pop_n = nullptr;
  llvm::Function *gc_collect_fn = nullptr;
};

extern RuntimeIR runtime_ir;
void init_runtime_ir(llvm::Module *M, llvm::LLVMContext &C);

#endif

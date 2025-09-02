#ifndef RUNTIME_IR_H
#define RUNTIME_IR_H
#include <llvm/IR/Function.h>

static inline llvm::PointerType *getInt8PtrTy(llvm::LLVMContext &the_context,
                                              unsigned AddressSpace = 0) {
  return llvm::PointerType::get(llvm::Type::getInt8Ty(the_context),
                                AddressSpace);
}

struct RuntimeIR {
  llvm::Function *box_nil = nullptr;
  llvm::Function *box_bool = nullptr;
  llvm::Function *box_number = nullptr;
  llvm::Function *box_string = nullptr;
  llvm::Function *box_symbol = nullptr;

  llvm::Function *unbox_bool = nullptr;
  llvm::Function *unbox_number = nullptr;
  llvm::Function *unbox_string = nullptr;
  llvm::Function *unbox_symbol = nullptr;

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
void init_runtime_ir(llvm::Module *the_module, llvm::LLVMContext &the_context);

#endif

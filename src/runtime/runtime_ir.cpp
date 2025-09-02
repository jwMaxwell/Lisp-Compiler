#include "runtime_ir.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

RuntimeIR runtime_ir;

static llvm::Function *decl(llvm::Module *M, const char *name, llvm::Type *ret,
                            llvm::ArrayRef<llvm::Type *> params,
                            bool vararg = false) {
  auto *FT = llvm::FunctionType::get(ret, params, vararg);
  return llvm::cast<llvm::Function>(
      M->getOrInsertFunction(name, FT).getCallee());
}

void init_runtime_ir(llvm::Module *M, llvm::LLVMContext &C) {
  auto *I8Ptr = getInt8PtrTy(C);
  auto *I32 = llvm::Type::getInt32Ty(C);
  auto *Void = llvm::Type::getVoidTy(C);
  auto *F64 = llvm::Type::getDoubleTy(C);

  runtime_ir.box_nil = decl(M, "box_nil", I8Ptr, {});
  runtime_ir.box_bool = decl(M, "box_bool", I8Ptr, {I32});
  runtime_ir.box_number = decl(M, "box_number", I8Ptr, {F64});
  runtime_ir.box_string = decl(M, "box_string", I8Ptr, {I8Ptr});
  runtime_ir.box_symbol = decl(M, "box_symbol", I8Ptr, {I8Ptr});

  runtime_ir.unbox_bool = decl(M, "unbox_bool", I32, {I8Ptr});
  runtime_ir.unbox_number = decl(M, "unbox_number", F64, {I8Ptr});
  runtime_ir.unbox_string = decl(M, "unbox_string", I8Ptr, {I8Ptr});
  runtime_ir.unbox_symbol = decl(M, "unbox_symbol", I8Ptr, {I8Ptr});

  runtime_ir.cons = decl(M, "cons", I8Ptr, {I8Ptr, I8Ptr});
  runtime_ir.car = decl(M, "car", I8Ptr, {I8Ptr});
  runtime_ir.cdr = decl(M, "cdr", I8Ptr, {I8Ptr});
  runtime_ir.is_atom = decl(M, "is_atom", I8Ptr, {I8Ptr});
  runtime_ir.eq = decl(M, "eq", I8Ptr, {I8Ptr, I8Ptr});
  // runtime_ir.get_add = decl(M, "get_add", I8Ptr, {I8Ptr, I8Ptr});
  // runtime_ir.get_sub = decl(M, "get_sub", I8Ptr, {I8Ptr, I8Ptr});
  // runtime_ir.get_mul = decl(M, "get_mul", I8Ptr, {I8Ptr, I8Ptr});
  // runtime_ir.get_div = decl(M, "get_div", I8Ptr, {I8Ptr, I8Ptr});

  runtime_ir.print_value = decl(M, "print_value", Void, {I8Ptr});

  runtime_ir.gc_root_push = decl(M, "gc_root_push", Void, {I8Ptr});
  runtime_ir.gc_root_pop_n =
      decl(M, "gc_root_pop_n", Void, {llvm::Type::getInt64Ty(C)});
  runtime_ir.gc_collect_fn = decl(M, "gc_collect", Void, {});
}

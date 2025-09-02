#include "runtime_ir.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

RuntimeIR runtime_ir;

static llvm::Function *decl(llvm::Module *the_module, const char *name,
                            llvm::Type *ret,
                            llvm::ArrayRef<llvm::Type *> params,
                            bool vararg = false) {
  auto *FT = llvm::FunctionType::get(ret, params, vararg);
  return llvm::cast<llvm::Function>(
      the_module->getOrInsertFunction(name, FT).getCallee());
}

void init_runtime_ir(llvm::Module *the_module, llvm::LLVMContext &the_context) {
  auto *I8Ptr = getInt8PtrTy(the_context);
  auto *I32 = llvm::Type::getInt32Ty(the_context);
  auto *Void = llvm::Type::getVoidTy(the_context);
  auto *F64 = llvm::Type::getDoubleTy(the_context);

  runtime_ir.box_nil = decl(the_module, "box_nil", I8Ptr, {});
  runtime_ir.box_bool = decl(the_module, "box_bool", I8Ptr, {I32});
  runtime_ir.box_number = decl(the_module, "box_number", I8Ptr, {F64});
  runtime_ir.box_string = decl(the_module, "box_string", I8Ptr, {I8Ptr});
  runtime_ir.box_symbol = decl(the_module, "box_symbol", I8Ptr, {I8Ptr});

  runtime_ir.unbox_bool = decl(the_module, "unbox_bool", I32, {I8Ptr});
  runtime_ir.unbox_number = decl(the_module, "unbox_number", F64, {I8Ptr});
  runtime_ir.unbox_string = decl(the_module, "unbox_string", I8Ptr, {I8Ptr});
  runtime_ir.unbox_symbol = decl(the_module, "unbox_symbol", I8Ptr, {I8Ptr});

  runtime_ir.cons = decl(the_module, "cons", I8Ptr, {I8Ptr, I8Ptr});
  runtime_ir.car = decl(the_module, "car", I8Ptr, {I8Ptr});
  runtime_ir.cdr = decl(the_module, "cdr", I8Ptr, {I8Ptr});
  runtime_ir.is_atom = decl(the_module, "is_atom", I8Ptr, {I8Ptr});
  runtime_ir.eq = decl(the_module, "eq", I8Ptr, {I8Ptr, I8Ptr});

  runtime_ir.print_value = decl(the_module, "print_value", Void, {I8Ptr});

  runtime_ir.gc_root_push = decl(the_module, "gc_root_push", Void, {I8Ptr});
  runtime_ir.gc_root_pop_n = decl(the_module, "gc_root_pop_n", Void,
                                  {llvm::Type::getInt64Ty(the_context)});
  runtime_ir.gc_collect_fn = decl(the_module, "gc_collect", Void, {});
}

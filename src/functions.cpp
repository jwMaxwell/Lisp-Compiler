#include "llvm_lisp.h"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <string>

#include "functions.h"

llvm::Function *add_fn = nullptr;
llvm::Function *sub_fn = nullptr;
llvm::Function *mul_fn = nullptr;
llvm::Function *div_fn = nullptr;
llvm::Function *print_fn = nullptr;

llvm::Function *bin_math_fn(llvm::Module *module, llvm::LLVMContext &context,
                            std::string fn_name) {
    log_debug("bin_math_fn");
    if (!module)
        return log_error_f("module is nullptr");

    llvm::FunctionType *funcType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(context),
        {llvm::Type::getDoubleTy(context), llvm::Type::getDoubleTy(context)},
        false);

    llvm::Function *math_function = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, fn_name, module);

    llvm::BasicBlock *entry =
        llvm::BasicBlock::Create(context, "entry", math_function);
    llvm::IRBuilder<> builder(entry);

    llvm::Function::arg_iterator args = math_function->arg_begin();
    if (args == math_function->arg_end()) {
        return log_error_f("failed to get function arguments");
    }

    llvm::Value *arg1 = args++;
    llvm::Value *arg2 = args++;

    llvm::Value *result =
        fn_name == "add"   ? builder.CreateFAdd(arg1, arg2, "addresult")
        : fn_name == "sub" ? builder.CreateFSub(arg1, arg2, "subresult")
        : fn_name == "mul" ? builder.CreateFMul(arg1, arg2, "mulresult")
        : fn_name == "div" ? builder.CreateFDiv(arg1, arg2, "divresult")
                           : nullptr;

    if (result == nullptr)
        return log_error_f("bin math fn not found");
    builder.CreateRet(result);

    if (llvm::verifyFunction(*math_function, &llvm::errs())) {
        return log_error_f("function verification failed");
    }

    return math_function;
}

// llvm::Function *init_print_fn(llvm::Module *module,
//                               llvm::LLVMContext &context) {
//     log_debug("print_fn");
//     if (!module)
//         return log_error_f("module is nullptr");

//     llvm::FunctionType *printfnType = llvm::FunctionType::get(
//         llvm::Type::getInt32Ty(context), // return int
//         llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)), //
//         char* true // variadic
//     );
//     llvm::Function *printfn = llvm::cast<llvm::Function>(
//         module->getOrInsertFunction("printf", printfnType).getCallee());
//     return printfn;
// }

void init_functions(llvm::Module *module, llvm::LLVMContext &context) {
    log_debug("init_functions");
    add_fn = bin_math_fn(module, context, "add");
    sub_fn = bin_math_fn(module, context, "sub");
    mul_fn = bin_math_fn(module, context, "mul");
    div_fn = bin_math_fn(module, context, "div");
    // print_fn = init_print_fn(module, context);
}

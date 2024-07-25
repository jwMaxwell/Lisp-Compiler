#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include "nodes.h"
#include "llvm_lisp.h"

#include "functions.h"

llvm::Function* bin_math_fn(llvm::Module *module, llvm::LLVMContext &context, std::string fn_name) {
	log_debug("bin_math_fn");
	if (!module)
		return log_error_f("module is nullptr");

	llvm::FunctionType* funcType =
		llvm::FunctionType::get(llvm::Type::getFloatTy(context), {
				llvm::Type::getFloatTy(context),
				llvm::Type::getBFloatTy(context)
			}, false);

	llvm::Function* mathFunction =
		llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, fn_name, module);

	llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mathFunction);
	llvm::IRBuilder<> builder(entry);

	llvm::Function::arg_iterator args = mathFunction->arg_begin();
	if (args == mathFunction->arg_end()) {
		return log_error_f("failed to get function arguments");
	}

	llvm::Value* arg1 = args++;
	llvm::Value* prearg2 = args;

	// For some reason, the second arg was bfloat by default
	llvm::Value* arg2 = builder.CreateFPExt(prearg2, llvm::Type::getFloatTy(context), "arg2_float");


	llvm::Value* result = fn_name == "add"
		? builder.CreateFAdd(arg1, arg2, "addresult")
		: fn_name == "sub"
		? builder.CreateFSub(arg1, arg2, "subresult")
		: fn_name == "mul"
		? builder.CreateFMul(arg1, arg2, "mulresult")
		: fn_name == "div"
		? builder.CreateFDiv(arg1, arg2, "divresult")
		: nullptr;

	if (result == nullptr)
		return log_error_f("bin math fn not found");
	builder.CreateRet(result);

	if (llvm::verifyFunction(*mathFunction, &llvm::errs())) {
		return log_error_f("function verification failed");
	}

	return mathFunction;
}

llvm::Function* add_fn = nullptr;
llvm::Function* sub_fn = nullptr;
llvm::Function* mul_fn = nullptr;
llvm::Function* div_fn = nullptr;

void init_functions(llvm::Module* module, llvm::LLVMContext& context) {
	log_debug("init_functions");
	add_fn = bin_math_fn(module, context, "add");
	sub_fn = bin_math_fn(module, context, "sub");
	mul_fn = bin_math_fn(module, context, "mul");
	div_fn = bin_math_fn(module, context, "div");
}

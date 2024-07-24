#include <string>
#include <iostream>
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

llvm::Function* init_add_fn(llvm::Module* module, llvm::LLVMContext& context) {
	log_debug("init_add_fn");
	// Check if module is nullptr
	if (!module) {
		std::cerr << "ERROR: module is nullptr\n";
		return nullptr;
	}
    
	// Create the add function type: float add(float a, float b)
	llvm::FunctionType* funcType =
		llvm::FunctionType::get(llvm::Type::getFloatTy(context), {
				llvm::Type::getFloatTy(context),
				llvm::Type::getFloatTy(context)
			}, false);
    
	// Create the add function and set its name
	llvm::Function* addFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "add", module);
    
	// Create the entry block and set the insertion point
	llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", addFunction);
	llvm::IRBuilder<> builder(entry);
    
	// Get the function arguments
	llvm::Function::arg_iterator args = addFunction->arg_begin();
	if (args == addFunction->arg_end()) {
		std::cerr << "ERROR: Failed to get function arguments\n";
		return nullptr;
	}
    
	llvm::Value* arg1 = args++;
	llvm::Value* arg2 = args;
    
	// Create the add instruction and return the result
	llvm::Value* result = builder.CreateFAdd(arg1, arg2, "addresult");
	builder.CreateRet(result);
    
	// Validate the function
	if (llvm::verifyFunction(*addFunction, &llvm::errs())) {
		std::cerr << "ERROR: Function verification failed\n";
		return nullptr;
	}
    
	return addFunction;
}

llvm::Function* init_sub_fn(llvm::Module* module, llvm::LLVMContext& context) {
	log_debug("init_sub_fn");
	if (!module) {
		return log_error_f("module is nullptr");
	}
    
	llvm::FunctionType* funcType =
		llvm::FunctionType::get(llvm::Type::getFloatTy(context), {
				llvm::Type::getFloatTy(context),
				llvm::Type::getFloatTy(context)}, 
			false);
    
	llvm::Function* subFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "sub", module);
    
	llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", subFunction);
	llvm::IRBuilder<> builder(entry);
    
	llvm::Function::arg_iterator args = subFunction->arg_begin();
	if (args == subFunction->arg_end()) {
		std::cerr << "ERROR: Failed to get function arguments\n";
		return nullptr;
	}
    
	llvm::Value* arg1 = args++;
	llvm::Value* arg2 = args;
    
	llvm::Value* result = builder.CreateFSub(arg1, arg2, "addresult");
	builder.CreateRet(result);
    
	if (llvm::verifyFunction(*subFunction, &llvm::errs())) {
		std::cerr << "ERROR: Function verification failed\n";
		return nullptr;
	}
    
	return subFunction;
}

llvm::Value* call_function(llvm::Function* targetFunction, llvm::Value* arg1, llvm::Value* arg2) {
	log_debug("call_function");
	llvm::Value* result = the_builder.CreateCall(targetFunction, {arg1, arg2}, "callresult");
	return result;
}

llvm::Function* add_fn = nullptr;
llvm::Function* sub_fn = nullptr;

void init_functions(llvm::Module* module, llvm::LLVMContext& context) {
	log_debug("init_functions");
	add_fn = init_add_fn(module, context);
	sub_fn = init_sub_fn(module, context);
}

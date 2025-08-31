#ifndef EXPRESSION_CONTAINER_H_
#define EXPRESSION_CONTAINER_H_

#include "../llvm/llvm_lisp.h"
#include "Node.h"
#include "node_fwd.h"
#include <iostream>
#include <llvm/IR/IRBuilder.h>

class Expression_Container : public Node {
public:
  std::vector<node_ptr> children;
  Expression_Container() { class_name = "Expression_Container"; }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Expression_Container" << std::endl;
    for (const auto &expr : children) {
      expr->print(indent + 2);
    }
  }

  llvm::Value *codegen() override {
    log_debug("Expression_Container->codegen()");

    auto *return_type = llvm::Type::getInt32Ty(the_context);
    auto *function_type = llvm::FunctionType::get(return_type, {}, false);
    auto *function =
        llvm::Function::Create(function_type, llvm::Function::ExternalLinkage,
                               "main", the_module.get());
    auto *block = llvm::BasicBlock::Create(the_context, "entry", function);
    the_builder.SetInsertPoint(block);

    llvm::Value *lastValue = nullptr;
    for (const auto &expr : children) {
      lastValue = expr->codegen();
      if (!lastValue) {
        return log_error_v(
            "Failed to generate code for an expression in the sequence");
      }
    }
    the_builder.CreateRet(lastValue);
    return function;
  }
};

#endif // EXPRESSION_CONTAINER_H_

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "../llvm/llvm_lisp.h"
#include "../runtime/runtime_ir.h"
#include "Identifier_Literal.h"
#include "Literal.h"
#include "Node.h"
#include <iostream>

using node_ptr = std::shared_ptr<Node>;
class Expression : public Node {
public:
  std::vector<node_ptr> children;

  Expression() { class_name = "expression"; }

  std::string get_class_name() const override { return class_name; }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Expression" << std::endl;
    for (const auto &child : children) {
      child->print(indent + 2);
    }
  }

  llvm::Value *add_codegen() {
    log_debug("add_codegen()");

    llvm::Value *acc = children.at(1)->codegen();
    for (size_t i = 2; i < children.size(); ++i) {
      llvm::Value *next = children.at(i)->codegen();
      acc = the_builder.CreateCall(runtime_ir.get_add, {acc, next}, "add");
    }
    return acc;
  }

  llvm::Value *sub_codegen() {
    log_debug("sub_codegen()");

    llvm::Value *acc = children.at(1)->codegen();
    for (size_t i = 2; i < children.size(); ++i) {
      llvm::Value *next = children.at(i)->codegen();
      acc = the_builder.CreateCall(runtime_ir.get_sub, {acc, next}, "sub");
    }
    return acc;
  }

  llvm::Value *mul_codegen() {
    log_debug("mul_codegen()");

    llvm::Value *acc = children.at(1)->codegen();
    for (size_t i = 2; i < children.size(); ++i) {
      llvm::Value *next = children.at(i)->codegen();
      acc = the_builder.CreateCall(runtime_ir.get_mul, {acc, next}, "mul");
    }
    return acc;
  }

  llvm::Value *div_codegen() {
    log_debug("div_codegen()");

    llvm::Value *acc = children.at(1)->codegen();
    for (size_t i = 2; i < children.size(); ++i) {
      llvm::Value *next = children.at(i)->codegen();
      acc = the_builder.CreateCall(runtime_ir.get_div, {acc, next}, "div");
    }
    return acc;
  }

  llvm::Value *quote_codegen(node_ptr n) {
    log_debug("quote_codegen()");
    if (n->get_class_name() == "numeric_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(the_context),
                                       std::stod(temp_node->get_value()));
      return the_builder.CreateCall(runtime_ir.number_handle, {val}, "numobj");
    } else if (n->get_class_name() == "string_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto stringPtr = the_builder.CreateGlobalString(temp_node->get_value());
      return the_builder.CreateCall(runtime_ir.string_handle, {stringPtr},
                                    "strobj");
    } else if (n->get_class_name() == "identifier_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto symbolPtr = the_builder.CreateGlobalString(temp_node->get_value());
      return the_builder.CreateCall(runtime_ir.symbol_handle, {symbolPtr},
                                    "symobj");
    } else if (n->get_class_name() == "expression") {
      std::shared_ptr<Expression> expr_node =
          std::dynamic_pointer_cast<Expression>(n);

      llvm::Value *acc = the_builder.CreateCall(runtime_ir.nil_handle, {});
      for (int i = (int)expr_node->children.size() - 1; i >= 0; --i) {
        llvm::Value *element = quote_codegen(expr_node->children[i]);
        acc = the_builder.CreateCall(runtime_ir.cons, {element, acc}, "cons");
      }
      return acc;
    }
    return log_error_v("Type not supported by quote");
  }

  llvm::Value *codegen() override {
    log_debug("Expression->codegen()");

    if (children.empty()) {
      return log_error_v("infertile expression");
    }

    if (auto id =
            std::dynamic_pointer_cast<Identifier_Literal>(children.at(0))) {
      if (id->get_value() == "quote") {
        if (children.size() < 2)
          return log_error_v("quote cannot be empty");
        return quote_codegen(children.at(1));
      } else if (id->get_value() == "+") {
        if (children.size() < 2)
          return log_error_v("Cannot Add nothing");
        return add_codegen();
      } else if (id->get_value() == "-") {
        if (children.size() < 2)
          return log_error_v("Cannot Add nothing");
        return sub_codegen();
      } else if (id->get_value() == "*") {
        if (children.size() < 2)
          return log_error_v("Cannot Add nothing");
        return mul_codegen();
      } else if (id->get_value() == "/") {
        if (children.size() < 2)
          return log_error_v("Cannot Add nothing");
        return div_codegen();
      }
    }

    llvm::Value *fn = children.at(0)->codegen();
    if (!fn) {
      return log_error_v("invalid function");
    }

    std::vector<llvm::Value *> args;
    for (size_t i = 1; i < children.size(); ++i) {
      llvm::Value *arg = children.at(i)->codegen();
      if (!arg) {
        return log_error_v("invalid argument");
      }
      args.push_back(arg);
    }

    llvm::Function *func = llvm::dyn_cast<llvm::Function>(fn);
    if (func->getReturnType()->isVoidTy()) {
      the_builder.CreateCall(func, args);
      return the_builder.getInt32(0);
    } else {
      return the_builder.CreateCall(func, args, "callresult");
    }
  }
};

#endif // EXPRESSION_H_

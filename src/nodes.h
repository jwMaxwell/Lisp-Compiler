#ifndef NODES_H
#define NODES_H

#include "functions.h"
#include "llvm_lisp.h"
#include "runtime.h"
#include "runtime_ir.h"
#include "tokenizer.h"
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <vector>

class Node {
protected:
  std::string class_name;

public:
  virtual ~Node() = default;
  virtual std::string get_class_name() const { return class_name; }
  virtual void print(int indent = 0) const = 0;
  virtual llvm::Value *codegen() = 0;
};

class Literal : public Node {
protected:
  std::string value;
  std::string type;
  size_t line;
  size_t column;

public:
  Literal(const token_t &tok)
      : value(tok.value), type(tok.type), line(tok.line), column(tok.column) {
    class_name = "literal";
  }

  void set_value(std::string _value) { value = _value; }

  const std::string get_value() const { return value; }

  const std::string get_type() const { return type; }

  size_t get_line() const { return line; }

  size_t get_column() const { return column; }

  std::string get_class_name() const override { return class_name; }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Literal: " << value << " (Type: " << type
              << ", Line: " << line << ", Column: " << column << ")"
              << std::endl;
  }

  llvm::Value *codegen() override {
    log_debug("Literal->codegen()");
    return nullptr;
  }
};

class String_Literal : public Literal {
public:
  String_Literal(const token_t &tok) : Literal(tok) {
    class_name = "string_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "String_Literal: \"" << value << "\""
              << std::endl;
  }

  llvm::Value *codegen() override {
    log_debug("String_Literal->codegen()");

    auto stringPtr = the_builder.CreateGlobalString(value);
    return the_builder.CreateCall(runtime_ir.string_handle, {stringPtr},
                                  "strobj");
  }
};

class Numeric_Literal : public Literal {
public:
  Numeric_Literal(const token_t &tok) : Literal(tok) {
    class_name = "numeric_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Numeric_Literal: " << value << std::endl;
  }

  llvm::Value *codegen() override {
    log_debug("Numeric_Literal->codegen()");
    print(1);

    auto val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(the_context),
                                     std::stod(value));
    return the_builder.CreateCall(runtime_ir.number_handle, {val}, "numobj");
  }
};

class Identifier_Literal : public Literal {
public:
  Identifier_Literal(const token_t &tok) : Literal(tok) {
    class_name = "identifier_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Identifier_Literal: " << value << std::endl;
  }

  llvm::Value *codegen() override {
    log_debug("Identifier_Literal->codegen()");
    if (value == "+")
      return add_fn;
    else if (value == "-")
      return sub_fn;
    else if (value == "*")
      return mul_fn;
    else if (value == "/")
      return div_fn;
    else if (value == "print")
      return runtime_ir.print_value;
    else if (value == "cons")
      return runtime_ir.cons;
    else if (value == "car")
      return runtime_ir.car;
    else if (value == "cdr")
      return runtime_ir.cdr;
    // else if (value == "atom?")
    //   return runtime_ir.is_atom;
    // else if (value == "eq?")
    //   return runtime_ir.eq;

    std::string error = "identifier does not map to any given function: ";
    error += value;
    return log_error_f(error.c_str());
  }
};

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

#endif // NODES_H

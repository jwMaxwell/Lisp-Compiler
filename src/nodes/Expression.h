#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "../llvm/llvm_lisp.h"
#include "../runtime/runtime_ir.h"
#include "Identifier_Literal.h"
#include "Literal.h"
#include "Node.h"
#include "codegen_result.h"
#include "functions.h"
#include "node_fwd.h"
#include <algorithm>
#include <iostream>

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

  // TODO: Add gc
  CodegenResult quote_codegen(node_ptr n) {
    log_debug("quote_codegen()");
    if (n->get_class_name() == "numeric_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(the_context),
                                       std::stod(temp_node->get_value()));
      llvm::Value *res =
          the_builder.CreateCall(runtime_ir.box_number, {val}, "numobj");
      return {res, res->getType(), true};
    } else if (n->get_class_name() == "string_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto stringPtr = the_builder.CreateGlobalString(temp_node->get_value());
      llvm::Value *res =
          the_builder.CreateCall(runtime_ir.box_string, {stringPtr}, "strobj");
      return {res, res->getType(), true};
    } else if (n->get_class_name() == "identifier_literal") {
      std::shared_ptr<Literal> temp_node =
          std::dynamic_pointer_cast<Literal>(n);
      auto symbolPtr = the_builder.CreateGlobalString(temp_node->get_value());
      llvm::Value *res =
          the_builder.CreateCall(runtime_ir.box_symbol, {symbolPtr}, "symobj");
      return {res, res->getType(), true};
    } else if (n->get_class_name() == "expression") {
      std::shared_ptr<Expression> expr_node =
          std::dynamic_pointer_cast<Expression>(n);

      llvm::Value *acc = the_builder.CreateCall(runtime_ir.box_nil, {});
      for (int i = (int)expr_node->children.size() - 1; i >= 0; --i) {
        CodegenResult element = quote_codegen(expr_node->children[i]);
        acc = the_builder.CreateCall(runtime_ir.cons,
                                     {box_value(element.value), acc}, "cons");
      }
      return {acc, acc->getType(), true};
    }
    return log_error_v("Type not supported by quote");
  }

  // TODO: Function has too many responsabilities
  CodegenResult codegen() override {
    log_debug("Expression->codegen()");

    if (children.empty()) {
      return log_error_v("infertile expression");
    }

    std::string fn_name =
        std::dynamic_pointer_cast<Identifier_Literal>(children.at(0))
            ->get_value();
    if (fn_name == "quote") {
      return children.size() < 2 ? log_error_v("quote cannot be empty")
                                 : quote_codegen(children.at(1));
    }

    std::vector<CodegenResult> args;
    size_t protected_count = 0;
    auto *i8ptr = getInt8PtrTy(the_context);
    for (size_t i = 1; i < children.size(); ++i) {
      CodegenResult arg = children.at(i)->codegen();
      if (!arg.value)
        return log_error_v("invalid argument");

      if (arg.isBoxed) {
        llvm::Value *arg_i8 =
            the_builder.CreateBitCast(arg.value, i8ptr, "root_cast");
        the_builder.CreateCall(runtime_ir.gc_root_push, {arg_i8});
        ++protected_count;
      }

      args.push_back(arg);
    }

    CodegenResult result = {};
    if (fn_name == "+" || fn_name == "-" || fn_name == "*" || fn_name == "/") {
      std::vector<CodegenResult> nums;
      for (auto a : args) {
        if (a.type->isDoubleTy()) {
          nums.push_back(a);
        } else if (a.type->isPointerTy()) {
          llvm::Value *ai8 = the_builder.CreateBitCast(a.value, i8ptr);
          llvm::Value *unboxed = the_builder.CreateCall(runtime_ir.unbox_number,
                                                        {ai8}, "unbox_num");
          nums.push_back({unboxed, unboxed->getType(), false});
        } else {
          return log_error_v("unsupported arg type for +");
        }
      }

      if (fn_name == "+") {
        result = add_codegen(nums);
      } else if (fn_name == "-") {
        result = sub_codegen(nums);
      } else if (fn_name == "*") {
        result = mul_codegen(nums);
      } else if (fn_name == "/") {
        result = div_codegen(nums);
      }

      if (result.value != nullptr)
        return result;
    } else {
      llvm::Value *fn = children.at(0)->codegen().value;
      if (!fn)
        return log_error_v("invalid function");

      llvm::Function *func = llvm::dyn_cast<llvm::Function>(fn);
      std::vector<llvm::Value *> boxed_args;
      for (auto a : args) {
        llvm::Value *boxed = nullptr;
        if (a.type->isDoubleTy()) {
          boxed = the_builder.CreateCall(runtime_ir.box_number, {a.value},
                                         "box_arg");
        } else if (a.type->isPointerTy()) {
          boxed = the_builder.CreateBitCast(a.value, i8ptr, "ptr_to_i8");
        } else {
          return log_error_v("unsupported arg type for runtime call");
        }
        boxed_args.push_back(boxed);
      }

      if (func->getReturnType()->isVoidTy()) {
        the_builder.CreateCall(func, boxed_args);
        auto *temp = the_builder.getInt32(0);
        result = {temp, temp->getType(), false};
      } else {
        auto *temp = the_builder.CreateCall(func, boxed_args, "callresult");
        result = {temp, temp->getType(), true};
      }
    }

    the_builder.CreateCall(
        runtime_ir.gc_root_pop_n,
        {llvm::ConstantInt::get(llvm::Type::getInt64Ty(the_context),
                                protected_count)});

    /* the_builder.CreateCall(runtime_ir.gc_collect_fn, {}); */

    return result;
  }
};

#endif // EXPRESSION_H_

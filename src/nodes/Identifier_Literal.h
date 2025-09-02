#ifndef IDENTIFIER_LITERAL_H_
#define IDENTIFIER_LITERAL_H_

#include "../llvm/llvm_lisp.h"
#include "../parser/tokenizer.h"
#include "../runtime/runtime_ir.h"
#include "Literal.h"
#include "codegen_result.h"
#include <iostream>
#include <llvm/IR/Value.h>

class Identifier_Literal : public Literal {
public:
  Identifier_Literal(const token_t &tok) : Literal(tok) {
    class_name = "identifier_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Identifier_Literal: " << value << std::endl;
  }

  CodegenResult codegen() override {
    log_debug("Identifier_Literal->codegen()");

    llvm::Value *res = nullptr;
    if (value == "print") {
      res = runtime_ir.print_value;
    } else if (value == "cons") {
      res = runtime_ir.cons;
    } else if (value == "car") {
      res = runtime_ir.car;
    } else if (value == "cdr") {
      res = runtime_ir.cdr;
    } else if (value == "atom?") {
      res = runtime_ir.is_atom;
    } else if (value == "eq?") {
      res = runtime_ir.eq;
    }

    if (res != nullptr) {
      return {res, res->getType(), false};
    }

    std::string error = "identifier does not map to any given function: ";
    error += value;
    return log_error_f(error.c_str());
  }
};

#endif // IDENTIFIER_LITERAL_H_

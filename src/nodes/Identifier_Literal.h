#ifndef IDENTIFIER_LITERAL_H_
#define IDENTIFIER_LITERAL_H_

#include "../llvm/llvm_lisp.h"
#include "../parser/tokenizer.h"
#include "../runtime/runtime_ir.h"
#include "Literal.h"
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

  llvm::Value *codegen() override {
    log_debug("Identifier_Literal->codegen()");
    if (value == "print")
      return runtime_ir.print_value;
    else if (value == "cons")
      return runtime_ir.cons;
    else if (value == "car")
      return runtime_ir.car;
    else if (value == "cdr")
      return runtime_ir.cdr;
    else if (value == "atom?")
      return runtime_ir.is_atom;
    else if (value == "eq?")
      return runtime_ir.eq;

    std::string error = "identifier does not map to any given function: ";
    error += value;
    return log_error_f(error.c_str());
  }
};

#endif // IDENTIFIER_LITERAL_H_

#ifndef NUMERIC_LITERAL_H_
#define NUMERIC_LITERAL_H_

#include "../runtime/runtime_ir.h"
#include "Literal.h"
#include <iostream>
#include <llvm/IR/Value.h>

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

#endif // NUMERIC_LITERAL_H_

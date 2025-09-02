#ifndef NUMERIC_H_
#define NUMERIC_H_

#include "../../runtime/runtime_ir.h"
#include "../codegen_result.h"
#include "Literal.h"
#include <iostream>
#include <llvm/IR/Value.h>

class Numeric : public Literal {
public:
  Numeric(const token_t &tok) : Literal(tok) { class_name = "numeric_literal"; }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Numeric_Literal: " << value << std::endl;
  }

  /* llvm::Value *codegen() override { */
  /*   log_debug("Numeric_Literal->codegen()"); */
  /*   print(1); */

  /*   auto val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(the_context), */
  /*                                    std::stod(value)); */
  /*   return val; */
  /* } */

  CodegenResult codegen() override {
    log_debug("Numeric_Literal->codegen()");
    print(1);

    auto val = llvm::ConstantFP::get(llvm::Type::getDoubleTy(the_context),
                                     std::stod(value));
    return {val, llvm::Type::getDoubleTy(the_context), false};
  }
};

#endif // NUMERIC_H_

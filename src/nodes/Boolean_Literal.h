#ifndef BOOLEAN_LITERAL_H_
#define BOOLEAN_LITERAL_H_

#include "../llvm/llvm_lisp.h"
#include "../parser/tokenizer.h"
#include "../runtime/runtime_ir.h"
#include "Literal.h"
#include <iostream>
#include <llvm/IR/Value.h>

class Boolean_Literal : public Literal {
public:
  Boolean_Literal(const token_t &tok) : Literal(tok) {
    class_name = "boolen_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Boolean_Literal: " << value << std::endl;
  }

  llvm::Value *codegen() override {
    log_debug("Boolean_Literal->codegen()");
    print(1);

    int temp = value == "true" ? 1 : 0;
    log_debug("Boolean_Literal->codegen()");
    auto val =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(the_context), temp);
    return the_builder.CreateCall(runtime_ir.bool_handle, {val}, "boolobj");
  }
};

#endif // BOOLEAN_LITERAL_H_

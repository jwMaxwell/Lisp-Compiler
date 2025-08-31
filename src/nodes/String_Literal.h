#ifndef STRING_LITERAL_H_
#define STRING_LITERAL_H_

#include "../runtime/runtime_ir.h"
#include "Literal.h"
#include <iostream>
#include <llvm/IR/Value.h>

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

#endif // STRING_LITERAL_H_

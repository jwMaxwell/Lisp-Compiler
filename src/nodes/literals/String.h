#ifndef STRING_H_
#define STRING_H_

#include "../../runtime/runtime_ir.h"
#include "../codegen_result.h"
#include "Literal.h"
#include <iostream>
#include <llvm/IR/Value.h>

class String : public Literal {
public:
  String(const token_t &tok) : Literal(tok) { class_name = "string_literal"; }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "String_Literal: \"" << value << "\""
              << std::endl;
  }

  CodegenResult codegen() override {
    log_debug("String_Literal->codegen()");

    auto stringPtr = the_builder.CreateGlobalString(value);
    auto boxed_val =
        the_builder.CreateCall(runtime_ir.box_string, {stringPtr}, "strobj");
    return {boxed_val, boxed_val->getType(), true};
  }
};

#endif // STRING_H_

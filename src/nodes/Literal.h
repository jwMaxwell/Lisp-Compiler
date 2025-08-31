#ifndef LITERAL_H_
#define LITERAL_H_

#include "Node.h"
#include <iostream>
#include <llvm/IR/Value.h>

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

#endif // LITERAL_H_

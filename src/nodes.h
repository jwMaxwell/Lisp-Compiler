#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "tokenizer.h"

class Node {
protected:
  std::string className;

public:
  virtual ~Node() = default;
  virtual std::string getClassName() const {
    return className;
  }
  virtual void print(int indent = 0) const = 0;
};

using NodePtr = std::shared_ptr<Node>;
class Expression : public Node {
public:
  std::vector<NodePtr> children;

  Expression() {
    className = "expression";
  }

  std::string getClassName() const override {
    return className;
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Expression" << std::endl;
    for (const auto& child : children) {
      child->print(indent + 2);
    }
  }
};

class Literal : public Node {
protected:
  std::string value;
  std::string type;
  size_t line;
  size_t column;

public:
  Literal(const token_t& tok)
    : value(tok.value), type(tok.type), line(tok.line), column(tok.column) {
    className = "literal";
  }

  const void setValue(std::string _value) {
    value = _value;
  }

  const std::string getValue() const {
    return value;
  }

  const std::string getType() const {
    return type;
  }

  size_t getLine() const {
    return line;
  }

  size_t getColumn() const {
    return column;
  }

  std::string getClassName() const override {
    return className;
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Literal: " << value << " (Type: " << type
              << ", Line: " << line << ", Column: " << column << ")" << std::endl;
  }
};

class StringLiteral : public Literal {
public:
  StringLiteral(const token_t& tok) : Literal(tok) {
    className = "string_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "StringLiteral: \"" << value << "\"" << std::endl;
  }
};

class NumericLiteral : public Literal {
public:
  NumericLiteral(const token_t& tok) : Literal(tok) {
    className = "numeric_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "NumericLiteral: " << value << std::endl;
  }
};

class IdentifierLiteral : public Literal {
public:
  IdentifierLiteral(const token_t& tok) : Literal(tok) {
    className = "identifier_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "IdentifierLiteral: " << value << std::endl;
  }
};

#endif // NODES_H

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
  public:
  token_t token;

  Literal(const token_t& tok) : token(tok) {
    className = "literal";
  }

  std::string getClassName() const override {
    return className;
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Literal: " << token.value << std::endl;
  }
};

#endif // NODES_H

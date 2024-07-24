#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include "tokenizer.h"
#include "llvm_lisp.h"
#include "functions.h"

class Node {
protected:
  std::string className;

public:
  virtual ~Node() = default;
  virtual std::string getClassName() const {
    return className;
  }
  virtual void print(int indent = 0) const = 0;
	virtual llvm::Value* codegen() = 0;
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

  void setValue(std::string _value) {
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

	llvm::Value* codegen() override {
		LogDebug("Literal->codegen()");
		return nullptr;
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

	llvm::Value* codegen() override {
		LogDebug("StringLiteral->codegen()");
		return nullptr;
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

	llvm::Value* codegen() override {
		LogDebug("NumericLiteral->codegen()");
		print(1);
	  return llvm::ConstantFP::get(TheContext, llvm::APFloat(std::stod(value)));
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

	llvm::Value* codegen() override {
		LogDebug("IdentifierLiteral->codegen()");
		if (value == "+")
			return addFn;
		else if (value == "-")
			return subFn;
		return LogErrorF("identifier does not map to any given function");
	}
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

	llvm::Value* codegen() override {
		LogDebug("Expression->codegen()");

		if (children.empty()) {
			return LogErrorV("infertile expression");
		}

		llvm::Value* fn = children.at(0)->codegen();
		if (!fn) {
			return LogErrorV("invalid function");
		}

		std::vector<llvm::Value*> args;
		for (size_t i = 1; i < children.size(); ++i) {
			llvm::Value* arg = children.at(i)->codegen();
			if (!arg) {
				return LogErrorV("invalid argument");
			}
			args.push_back(arg);
		}

		if (llvm::Function* func = llvm::dyn_cast<llvm::Function>(fn)) {
        return Builder.CreateCall(func, args, "callresult");
    } else {
			return LogErrorV("invalid function");
		}
	}
};

class ExpressionContainer : public Node {
public:
	std::vector<NodePtr> children;
	ExpressionContainer() {
		className = "ExpressionContainer";
	}

	void print(int indent = 0) const override {
		std::string indentation(indent, ' ');
		std::cout << indentation << "ExpressionContainer" << std::endl;
		for (const auto& expr : children) {
			expr->print(indent + 2);
		}
	}

	llvm::Value* codegen() override {
		LogDebug("MainExpression->codegen()");

		llvm::Value* lastValue = nullptr;
		for (const auto& expr : children) {
			lastValue = expr->codegen();
			if (!lastValue) {
				return LogErrorV("Failed to generate code for an expression in the sequence");
			}
		}
		return lastValue;
	}
};

#endif // NODES_H

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
  std::string class_name;

public:
  virtual ~Node() = default;
  virtual std::string get_class_name() const {
    return class_name;
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
    class_name = "literal";
  }

  void set_value(std::string _value) {
    value = _value;
  }

  const std::string get_value() const {
    return value;
  }

  const std::string get_type() const {
    return type;
  }

  size_t get_line() const {
    return line;
  }

  size_t get_column() const {
    return column;
  }

  std::string get_class_name() const override {
    return class_name;
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Literal: " << value << " (Type: " << type
              << ", Line: " << line << ", Column: " << column << ")" << std::endl;
  }

	llvm::Value* codegen() override {
		log_debug("Literal->codegen()");
		return nullptr;
	}
};

class String_Literal : public Literal {
public:
  String_Literal(const token_t& tok) : Literal(tok) {
    class_name = "string_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "String_Literal: \"" << value << "\"" << std::endl;
  }

	llvm::Value* codegen() override {
		log_debug("String_Literal->codegen()");
		return nullptr;
	}
};

class Numeric_Literal : public Literal {
public:
  Numeric_Literal(const token_t& tok) : Literal(tok) {
    class_name = "numeric_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Numeric_Literal: " << value << std::endl;
  }

	llvm::Value* codegen() override {
		log_debug("Numeric_Literal->codegen()");
		print(1);
	  return llvm::ConstantFP::get(the_context, llvm::APFloat(std::stod(value)));
	}
};

class Identifier_Literal : public Literal {
public:
  Identifier_Literal(const token_t& tok) : Literal(tok) {
    class_name = "identifier_literal";
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Identifier_Literal: " << value << std::endl;
  }

	llvm::Value* codegen() override {
		log_debug("Identifier_Literal->codegen()");
		if (value == "+")
			return add_fn;
		else if (value == "-")
			return sub_fn;
		else if (value == "*")
			return mul_fn;
		else if (value == "/")
			return div_fn;
		return log_error_f("identifier does not map to any given function");
	}
};

using node_ptr = std::shared_ptr<Node>;
class Expression : public Node {
public:
  std::vector<node_ptr> children;

  Expression() {
    class_name = "expression";
  }

  std::string get_class_name() const override {
    return class_name;
  }

  void print(int indent = 0) const override {
    std::string indentation(indent, ' ');
    std::cout << indentation << "Expression" << std::endl;
    for (const auto& child : children) {
      child->print(indent + 2);
    }
  }

	llvm::Value* codegen() override {
		log_debug("Expression->codegen()");

		if (children.empty()) {
			return log_error_v("infertile expression");
		}

		llvm::Value* fn = children.at(0)->codegen();
		if (!fn) {
			return log_error_v("invalid function");
		}

		std::vector<llvm::Value*> args;
		for (size_t i = 1; i < children.size(); ++i) {
			llvm::Value* arg = children.at(i)->codegen();
			if (!arg) {
				return log_error_v("invalid argument");
			}
			args.push_back(arg);
		}

		if (llvm::Function* func = llvm::dyn_cast<llvm::Function>(fn)) {
        return the_builder.CreateCall(func, args, "callresult");
    } else {
			return log_error_v("invalid function");
		}
	}
};

class Expression_Container : public Node {
public:
	std::vector<node_ptr> children;
	Expression_Container() {
		class_name = "Expression_Container";
	}

	void print(int indent = 0) const override {
		std::string indentation(indent, ' ');
		std::cout << indentation << "Expression_Container" << std::endl;
		for (const auto& expr : children) {
			expr->print(indent + 2);
		}
	}

	llvm::Value* codegen() override {
		log_debug("MainExpression->codegen()");

		llvm::Value* lastValue = nullptr;
		for (const auto& expr : children) {
			lastValue = expr->codegen();
			if (!lastValue) {
				return log_error_v("Failed to generate code for an expression in the sequence");
			}
		}
		return lastValue;
	}
};

#endif // NODES_H

#ifndef NODE_H_
#define NODE_H_

#include "codegen_result.h"
#include <llvm/IR/Value.h>

class Node {
protected:
  std::string class_name;

public:
  virtual ~Node() = default;
  virtual std::string get_class_name() const { return class_name; }
  virtual void print(int indent = 0) const = 0;
  virtual CodegenResult codegen() = 0;
};

#endif // NODE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tokenizer.h"
#include "parser.h"
#include "nodes.h"
#include "llvm_lisp.h"
#include "functions.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Fatal error: no input files\n";
    return 1;
  }

  std::ifstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "Fatal error: could not open file " << argv[1] << std::endl;
		return 1;
	}
	
  std::string input(
    (std::istreambuf_iterator<char>(file)), 
    std::istreambuf_iterator<char>()
  );

  if (input.size() == 0) {
    std::cerr << "Error: empty input file" << std::endl;
    return 1;
  }

  std::vector<token_t> matches = tokenize(input);
  if (matches.size() == 0) {
    return 1;
  }

	std::cout << "============== Tokens ==============" << std::endl;
  for (const auto& match : matches) {
    std::cout << "{value: \"" << match.value
      << "\", type: \"" << match.type 
      << "\", line: " << match.line 
      << ", col: " << match.column << "},\n";
  }

	std::cout << "======= Abstract Syntax Tree =======" << std::endl;
  NodePtr ast = parse(matches);
  ast->print();

	std::cout << "============= LLVM IR ==============" << std::endl;
	initLLVM();
	initFunctions(TheModule.get(), TheContext);
	llvm::Value* ir_code = std::dynamic_pointer_cast<Expression>(ast)->codegen();
	ir_code->getName(); // gets rid of the warning
	// TheModule->print(llvm::outs(), nullptr);
	TheModule->print(llvm::errs(), nullptr);
	
  return 0;
}

#include "functions.h"
#include "llvm_lisp.h"
#include "nodes.h"
#include "parser.h"
#include "runtime_ir.h"
#include "tokenizer.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Support/SourceMgr.h"

int log_fatal_error(std::string error_message);

int main(int argc, char **argv) {
  if (argc < 2) {
    return log_fatal_error("no input files");
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    return log_fatal_error("could not open file");
  }

  std::string input((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

  if (input.size() == 0) {
    return log_fatal_error("empty input file");
  }

  std::vector<token_t> matches = tokenize(input);
  if (matches.size() == 0) {
    return log_fatal_error("file could not be tokenized");
  }

  std::cout << "============== Tokens ==============" << std::endl;
  for (const auto &match : matches) {
    std::cout << "{value: \"" << match.value << "\", type: \"" << match.type
              << "\", line: " << match.line << ", col: " << match.column
              << "},\n";
  }

  std::cout << "======= Abstract Syntax Tree =======" << std::endl;
  node_ptr ast = parse(matches);
  ast->print();

  std::cout << "============= LLVM IR ==============" << std::endl;
  init_llvm();
  init_functions(the_module.get(), the_context);
  init_runtime_ir(the_module.get(), the_context);

  // // Link runtime
  // llvm::SMDiagnostic err;
  // llvm::LLVMContext context;
  // auto runtimeModule = llvm::parseIRFile("src/runtime.bc", err, context);
  // if (!runtimeModule) {
  //   std::cerr << "Error parsing runtime.o\n";
  //   return 1;
  // }
  // llvm::Linker::linkModules(*the_module, std::move(runtimeModule));

  llvm::Value *ir_code =
      std::dynamic_pointer_cast<Expression_Container>(ast)->codegen();
  ir_code->getName(); // gets rid of the warning
  the_module->print(llvm::errs(), nullptr);

  // Generate LLVM IR file
  std::error_code ec;
  llvm::raw_fd_ostream out("target.ll", ec);
  the_module->print(out, nullptr);
  out.flush();

  return 0;
}

int log_fatal_error(std::string error_message) {
  std::cerr << "Critical Error: " << error_message << std::endl;
  return 1;
}

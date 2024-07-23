#include "llvm_lisp.h"

llvm::LLVMContext TheContext;
llvm::IRBuilder<> Builder(TheContext);
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value *> NamedValues;

void LogDebug(std::string stmt) {
	std::cout << "Debug: " << stmt << std::endl;
}

void LogError(std::string error) {
	std::cerr << "Error: " << error << std::endl;
}

llvm::Value* LogErrorV(const char *str) {
	LogError(str);
  return nullptr;
}

llvm::Function* LogErrorF(const char *str) {
	LogError(str);
	return nullptr;
}

void initLLVM() {
	TheModule = std::make_unique<llvm::Module>("JIT", TheContext);
}

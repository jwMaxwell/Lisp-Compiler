#include "llvm_lisp.h"

llvm::LLVMContext the_context;
llvm::IRBuilder<> the_builder(the_context);
std::unique_ptr<llvm::Module> the_module;
std::map<std::string, llvm::Value *> named_values;

void log_debug(std::string stmt) {
    std::cout << "Debug: " << stmt << std::endl;
}

void log_error(std::string error) {
    std::cerr << "Error: " << error << std::endl;
}

CodegenResult log_error_v(const char *str) {
    log_error(str);
    return {nullptr, nullptr, true};
}

CodegenResult log_error_f(const char *str) {
    log_error(str);
    return {nullptr, nullptr, true};
}

void init_llvm() {
    the_module = std::make_unique<llvm::Module>("JIT", the_context);
}

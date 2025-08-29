# Compiler
CXX = clang++

# LLVM Configuration
LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS = `$(LLVM_CONFIG) --cxxflags`
LLVM_LDFLAGS = `$(LLVM_CONFIG) --ldflags --system-libs --libs core`

# Compiler flags
CXXFLAGS = -std=c++20 -Wall $(LLVM_CXXFLAGS)

# Executable name
EXEC = driver.exe

# Source files
SRCS = src/driver.cpp src/tokenizer.cpp src/parser.cpp src/llvm_lisp.cpp src/functions.cpp src/runtime.cpp src/runtime_ir.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HDRS = src/tokenizer.h src/nodes.h src/parser.h src/llvm_lisp.h src/functions.h src/runtime.h src/runtime_ir.h

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LLVM_LDFLAGS)

# Rule to build object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean

# Compiler
CXX = clang++

# LLVM Configuration
LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS = `$(LLVM_CONFIG) --cxxflags`
LLVM_LDFLAGS = `$(LLVM_CONFIG) --ldflags --system-libs --libs core`

# Compiler flags
CXXFLAGS = -std=c++20 -Wall $(LLVM_CXXFLAGS)

# Executable name
EXEC = driver

# Source files
SRCS = src/driver.cpp \
src/parser/tokenizer.cpp \
src/parser/parser.cpp \
src/llvm/llvm_lisp.cpp \
src/runtime/runtime.cpp \
src/runtime/runtime_ir.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HDRS = src/parser/tokenizer.h \
src/nodes/Node.h \
src/nodes/Literal.h \
src/nodes/String_Literal.h \
src/nodes/Numeric_Literal.h \
src/nodes/Boolean_Literal.h \
src/nodes/Expression_Container.h \
src/parser/parser.h \
src/llvm/llvm_lisp.h \
src/runtime/runtime.h \
src/runtime/runtime_ir.h

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

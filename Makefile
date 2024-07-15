# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++20 -Wall

# Executable name
EXEC = driver.exe

# Source files
SRCS = src/driver.cpp src/tokenizer.cpp src/parser.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HDRS = src/tokenizer.h src/nodes.h src/parser.h

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean

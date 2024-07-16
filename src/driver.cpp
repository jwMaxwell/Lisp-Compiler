#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tokenizer.h"
#include "parser.h"
#include "nodes.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Fatal error: no input files\n";
    return 1;
  }

  std::ifstream file(argv[1]);
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

  /* Output Tokens */
  // for (const auto& match : matches) {
  //   std::cout << "{value: \"" << match.value
  //     << "\", type: \"" << match.type 
  //     << "\", line: " << match.line 
  //     << ", col: " << match.column << "},\n";
  // }

  NodePtr ast = buildAST(matches);
  ast = parseQuote(ast);
  ast->print();

  return 0;
}

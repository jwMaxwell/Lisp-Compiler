#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "nodes.h"
#include "tokenizer.h"

NodePtr buildAST(std::vector<token_t>& tokens, NodePtr ast = std::make_shared<Expression>());
NodePtr parseQuote(NodePtr ast);
NodePtr parse(std::vector<token_t>& tokens);

#endif // PARSER_H

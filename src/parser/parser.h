#ifndef PARSER_H
#define PARSER_H

#include "../nodes/nodes.h"
#include "tokenizer.h"
#include <vector>

node_ptr build_AST(std::vector<token_t> &tokens,
                   node_ptr ast = std::make_shared<Expression>());
node_ptr parse_quote(node_ptr ast);
node_ptr parse(std::vector<token_t> &tokens);

#endif // PARSER_H

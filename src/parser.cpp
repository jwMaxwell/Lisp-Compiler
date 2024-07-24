#include "parser.h"

node_ptr build_AST(std::vector<token_t>& tokens, node_ptr ast) {
  if (tokens.empty()) return ast;

  token_t token = tokens.front();
  tokens.erase(tokens.begin());

  if (token.value == "(" && token.type == "symbol") {
    node_ptr newExpression = std::make_shared<Expression>();
    std::dynamic_pointer_cast<Expression>(ast)->
      children.push_back(build_AST(tokens, newExpression));
    return build_AST(tokens, ast);
  } else if (token.value == ")" && token.type == "symbol") {
    return ast;
  } else {
    // omg LITERALly parsing
    node_ptr newLiteral;
    if (token.type == "number")
      newLiteral = std::make_shared<Numeric_Literal>(token);
    else if (token.type == "string")
      newLiteral = std::make_shared<String_Literal>(token);
    else if (token.type == "identifier")
      newLiteral = std::make_shared<Identifier_Literal>(token);

    std::dynamic_pointer_cast<Expression>(ast)->
      children.push_back(newLiteral);
    
    return build_AST(tokens, ast);
  }
}

node_ptr parse_quote(node_ptr ast) {
  std::shared_ptr<Expression> parent = std::dynamic_pointer_cast<Expression>(ast);
  if (parent) {
    for (size_t i = 0; i < parent->children.size(); ++i) {
      node_ptr& child = parent->children[i];
      if (child->get_class_name() == "expression") {
        parse_quote(child);
      } else if (child->get_class_name() == "identifier_literal") {
        std::shared_ptr<Literal> literalChild = std::dynamic_pointer_cast<Literal>(child);
        if (literalChild->get_value() == "'" && literalChild->get_type() == "identifier") {
          literalChild->set_value("quote");
          if (i + 1 < parent->children.size()) {
            std::vector<node_ptr> newQuoteChildren = {child, parent->children[i + 1]};
            node_ptr newExpression = std::make_shared<Expression>();
            std::dynamic_pointer_cast<Expression>(newExpression)->children = newQuoteChildren;
            parent->children[i] = newExpression;
            parent->children.erase(parent->children.begin() + i + 1);
          }
        }
      }
    }
  }
  return ast;
}

node_ptr parse(std::vector<token_t>& tokens) {
	node_ptr ast = build_AST(tokens);
	ast = parse_quote(ast);

	auto container = std::make_shared<Expression_Container>();
	container->children = 
		std::dynamic_pointer_cast<Expression>(ast)->children;

	return container;
}

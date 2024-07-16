#include "parser.h"

NodePtr buildAST(std::vector<token_t>& tokens, NodePtr ast) {
  if (tokens.empty()) return ast;

  token_t token = tokens.front();
  tokens.erase(tokens.begin());

  if (token.value == "(" && token.type == "symbol") {
    NodePtr newExpression = std::make_shared<Expression>();
    std::dynamic_pointer_cast<Expression>(ast)->
      children.push_back(buildAST(tokens, newExpression));
    return buildAST(tokens, ast);
  } else if (token.value == ")" && token.type == "symbol") {
    return ast;
  } else {
    // omg LITERALly parsing
    NodePtr newLiteral;
    if (token.type == "number")
      newLiteral = std::make_shared<NumericLiteral>(token);
    else if (token.type == "string")
      newLiteral = std::make_shared<StringLiteral>(token);
    else if (token.type == "identifier")
      newLiteral = std::make_shared<IdentifierLiteral>(token);

    std::dynamic_pointer_cast<Expression>(ast)->
      children.push_back(newLiteral);
    
    return buildAST(tokens, ast);
  }
}

NodePtr parseQuote(NodePtr ast) {
  std::shared_ptr<Expression> parent = std::dynamic_pointer_cast<Expression>(ast);
  if (parent) {
    for (size_t i = 0; i < parent->children.size(); ++i) {
      NodePtr& child = parent->children[i];
      if (child->getClassName() == "expression") {
        parseQuote(child);
      } else if (child->getClassName() == "identifier_literal") {
        std::shared_ptr<Literal> literalChild = std::dynamic_pointer_cast<Literal>(child);
        if (literalChild->getValue() == "'" && literalChild->getType() == "identifier") {
          literalChild->setValue("quote");
          if (i + 1 < parent->children.size()) {
            std::vector<NodePtr> newQuoteChildren = {child, parent->children[i + 1]};
            NodePtr newExpression = std::make_shared<Expression>();
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


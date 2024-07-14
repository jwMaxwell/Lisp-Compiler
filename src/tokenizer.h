#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

struct token_t {
    std::string value;
    std::string type;
    size_t line;
    size_t column;
};

std::vector<token_t> tokenize(const std::string& text);

#endif // TOKENIZER_H

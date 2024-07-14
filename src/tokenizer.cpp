#include "tokenizer.h"

std::vector<token_t> tokenize(const std::string& text) {
  std::vector<token_t> matches;
  std::regex regexPattern(R"(;.*?\n|\(|\)|"[^"]*"|'|[^\s()]+)");
  auto words_begin = std::sregex_iterator(text.begin(), text.end(), regexPattern);
  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    int match_pos = match.position();

    // Get token line number
    std::string temp = text.substr(0, match_pos);
    size_t line = std::count(temp.begin(), temp.end(), '\n') + 1;

    // Get token column number
    auto last_nl = temp.find_last_of("\n");
    size_t column = (last_nl == std::string::npos) ? match_pos + 1 : match_pos - last_nl;

    // Get token type
    std::string type = "identifier";
    std::string match_str = match.str();

    if (match_str[0] == ';') continue;
    else if (!match_str.empty() && std::find_if(match_str.begin(), 
      match_str.end(), [](unsigned char c) { return !std::isdigit(c); }) == match_str.end())
        type = "number";
    else if (match_str == "(" || match_str == ")")
      type = "symbol";
    else if (match_str[0] == '"' && match_str.back() == '"')
      type = "string";

    matches.push_back({ 
      type == "string" 
        ? match_str.substr(1, match_str.length() - 2) 
        : match_str, 
      type, 
      line, 
      column 
    });
  }

  return matches;
}

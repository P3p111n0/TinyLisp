#pragma once

#include "Result.h"
#include "Token.h"
#include <list>
#include <string>

class Tokenizer {
  public:
    Tokenizer() = default;
    Result<std::list<Token::Token>> tokenize(const std::string &) const;
  private:
    std::string _tokenize_string(std::ifstream &) const;
    void _skip_comment(std::ifstream &) const;
};

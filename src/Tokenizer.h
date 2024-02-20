#pragma once

#include "Result.h"
#include "Token.h"
#include <list>
#include <string>

class Tokenizer {
  protected:
    Tokenizer() = default;
    Result<std::list<Token::Token>> tokenize(std::istream &) const;
  private:
    std::string _tokenize_string(std::istream &) const;
    void _skip_comment(std::istream &) const;
};

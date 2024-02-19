#pragma once

#include "Result.h"
#include "Tokenizer.h"

class StdinTokenizer : public Tokenizer {
  public:
    StdinTokenizer() = default;
    Result<std::list<Token::Token>> tokenize() const;
};

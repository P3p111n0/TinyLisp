#pragma once

#include "Result.h"
#include "Tokenizer.h"

class FileTokenizer : public Tokenizer {
  public:
    FileTokenizer(std::string filename) : _filename(filename) {};
    Result<std::list<Token::Token>> tokenize() const;
  private:
    std::string _filename;
};
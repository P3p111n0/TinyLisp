#pragma once

#include "ASTNode.h"
#include "Result.h"
#include "Token.h"
#include <list>
#include <set>
#include <string>

class Parser {
  public:
    Parser();

    Result<std::list<std::shared_ptr<ASTNode>>> parse(std::list<Token>) const;

  private:
    Result<std::shared_ptr<ASTNode>>
    _parse(std::list<Token> &) const;
    Result<std::shared_ptr<ASTNode>> _parse_ex(std::list<Token> &) const;
    std::set<std::string> _builtin;
};
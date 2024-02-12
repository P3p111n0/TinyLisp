#pragma once

#include "ASTNode.h"
#include "Result.h"
#include "Token.h"
#include <list>
#include <set>
#include <string>
#include <functional>
#include <unordered_map>

class Parser {
  public:
    Parser();

    Result<std::list<std::shared_ptr<ASTNode>>> parse(std::list<Token::Token>) const;

  private:
    struct ParserFunctions {
        friend class Parser;
        static Result<std::shared_ptr<ASTNode>> _parse(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>> _parse_ex(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>> _parse_if(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_operator(std::stack<std::shared_ptr<ASTNode>> &, char);

        using ParseFunction = std::function<Result<std::shared_ptr<ASTNode>>(std::list<Token::Token> &)>;
        static std::unordered_map<std::string, ParseFunction> _parse_map;
    };

    std::set<std::string> _builtin;
};
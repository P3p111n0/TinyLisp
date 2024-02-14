#pragma once

#include "ASTNode.h"
#include "Result.h"
#include "Token.h"
#include <functional>
#include <list>
#include <set>
#include <string>
#include <unordered_map>

class Parser {
  public:
    Parser();

    Result<std::list<std::shared_ptr<ASTNode>>>
        parse(std::list<Token::Token>) const;

  private:
    struct ParserFunctions {
        static Result<std::shared_ptr<ASTNode>>
        _parse(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_ex(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_if(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_operator(std::stack<std::shared_ptr<ASTNode>> &, char);
        static inline bool
        _check_char(const std::list<Token::Token> &, char);
        static Result<std::shared_ptr<ASTNode>>
        _parse_cons(std::list<Token::Token> &);
        static Result<std::list<std::shared_ptr<ASTNode>>>
        _parse_args(std::list<Token::Token> &, int);
        static Result<std::shared_ptr<ASTNode>>
        _parse_car(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_cdr(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_lambda(std::list<Token::Token> &);
        static Result<std::shared_ptr<ASTNode>>
        _parse_defun(std::list<Token::Token> &);

        using ParseFunction = std::function<Result<std::shared_ptr<ASTNode>>(
            std::list<Token::Token> &)>;
        static std::unordered_map<std::string, ParseFunction> _parse_map;
    };

    std::set<std::string> _builtin;
};
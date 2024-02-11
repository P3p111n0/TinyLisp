#include "Parser.h"
#include <cassert>
#include <stack>

Parser::Parser() : _builtin({"let", "letrec", "lambda", "if", "nil", "cons"}) {}

Result<std::list<std::shared_ptr<ASTNode>>>
Parser::parse(std::list<Token> tokens) const {
    std::list<std::shared_ptr<ASTNode>> result;
    while (!tokens.empty()) {
        auto parsed_node = _parse(tokens);
        if (!parsed_node.valid()) {
            return {parsed_node.error()};
        }
        result.push_back(parsed_node.value());
    }
    return result;
}

Result<std::shared_ptr<ASTNode>>
Parser::_parse(std::list<Token> & tokens) const {
    assert(!tokens.empty());
    Token curr = tokens.front();
    tokens.pop_front();
    switch (curr.data.index()) {
    case 0: { // char
        switch (std::get<char>(curr.data)) {
        case '(':
            return _parse_ex(tokens);
        }
    }
    case 1: // int
        return {
            std::shared_ptr<ASTNode>(new ASTNodeInt(std::get<int>(curr.data)))};
    case 2: // string
        // TODO
        break;
    }

    return Result<std::shared_ptr<ASTNode>>(ParseError("Unknown token."));
}

Result<std::shared_ptr<ASTNode>>
Parser::_parse_ex(std::list<Token> & tokens) const {
    std::stack<std::shared_ptr<ASTNode>> s;
    while (!tokens.empty()) {
        auto token = tokens.front();
        tokens.pop_front();
        switch (token.data.index()) {
        case 0: { // char
            switch (std::get<char>(token.data)) {
            case '(': {
                if (auto ret = _parse_ex(tokens); ret.valid()) {
                    s.emplace(ret.value());
                } else {
                    return ret;
                }
                break;
            }
            case ')': {
                assert(s.size() == 1);
                return {s.top()};
            }
            case '+': {
                assert(s.size() >= 2);
                auto rhs = s.top();
                s.pop();
                auto lhs = s.top();
                s.pop();
                s.push(std::shared_ptr<ASTNode>(
                    new ASTNodeAdd(lhs, rhs)));
                break;
            }
            case '-': {
                assert(s.size() >= 2);
                auto rhs = s.top();
                s.pop();
                auto lhs = s.top();
                s.pop();
                s.push(std::shared_ptr<ASTNode>(
                    new ASTNodeSub(lhs, rhs)));
                break;
            }
            case '*': {
                assert(s.size() >= 2);
                auto rhs = s.top();
                s.pop();
                auto lhs = s.top();
                s.pop();
                s.push(std::shared_ptr<ASTNode>(
                    new ASTNodeMul(lhs, rhs)));
                break;
            }
            case '/': {
                assert(s.size() >= 2);
                auto rhs = s.top();
                s.pop();
                auto lhs = s.top();
                s.pop();
                s.push(std::shared_ptr<ASTNode>(
                    new ASTNodeDiv(lhs, rhs)));
                break;
            }
            }
            break;
        }
        case 1: // int
            s.push(std::shared_ptr<ASTNode>(
                new ASTNodeInt(std::get<int>(token.data))));
            break;
        case 2: // string
            break;
        }
    }

    assert(s.size() == 1);
    return {s.top()};
}
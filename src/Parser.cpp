#include "Parser.h"
#include <cassert>
#include <stack>

std::unordered_map<std::string, Parser::ParserFunctions::ParseFunction>
    Parser::ParserFunctions::_parse_map{
        {"if", Parser::ParserFunctions::_parse_if},
        {"cons", Parser::ParserFunctions::_parse_cons},
        {"true", [](std::list<Token::Token> &) {return std::shared_ptr<ASTNode>(new ASTNodeInt(1));}},
        {"false", [](std::list<Token::Token> &) {return std::shared_ptr<ASTNode>(new ASTNodeInt(0));}}
    };

inline bool Parser::ParserFunctions::_check_closing_parenthesis(
    const std::list<Token::Token> & tokens) {
    return tokens.front().index() == Token::TokenIndex::Char &&
           std::get<char>(tokens.front()) == ')';
}

Result<std::shared_ptr<ASTNode>>
Parser::ParserFunctions::_parse(std::list<Token::Token> & tokens) {
    assert(!tokens.empty());
    Token::Token curr = tokens.front();
    tokens.pop_front();
    switch (curr.index()) {
    case Token::TokenIndex::Char: {
        switch (std::get<char>(curr)) {
        case '(':
            return _parse_ex(tokens);
        case ')':
            return {ParseError("Unpaired closing parenthesis.")};
        }
    }
    case Token::TokenIndex::Int:
        return {std::shared_ptr<ASTNode>(new ASTNodeInt(std::get<int>(curr)))};
    case Token::TokenIndex::String: {
        std::string val = std::get<std::string>(curr);
        if (_parse_map.contains(val)) {
            auto parse_func = _parse_map[val];
            if (auto res = parse_func(tokens); !res.valid()) {
                return res;
            } else {
                return res.value();
            }
        } else {
            return {std::shared_ptr<ASTNode>(new ASTNodeIdentifier(val))};
        }
    }
    }

    return {(ParseError("Unknown token."))};
}

Result<std::shared_ptr<ASTNode>>
Parser::ParserFunctions::_parse_ex(std::list<Token::Token> & tokens) {
    std::stack<std::shared_ptr<ASTNode>> s;
    while (!tokens.empty()) {
        auto token = tokens.front();
        tokens.pop_front();
        switch (token.index()) {
        case Token::TokenIndex::Char: {
            switch (std::get<char>(token)) {
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
            case '+':
            case '-':
            case '*':
            case '/':
            case '>':
            case '<':
            case '=': {
                char op = std::get<char>(token);
                if (auto res = _parse_operator(s, op); !res.valid()) {
                    return res;
                } else {
                    s.push(res.value());
                }
            }
            }
            break;
        }
        case Token::TokenIndex::Int:
            s.push(
                std::shared_ptr<ASTNode>(new ASTNodeInt(std::get<int>(token))));
            break;
        case Token::TokenIndex::String: {
            std::string val = std::get<std::string>(token);
            if (_parse_map.contains(val)) {
                auto parse_func = _parse_map[val];
                if (auto res = parse_func(tokens); !res.valid()) {
                    return res;
                } else {
                    return res.value();
                }
            } else {
                return {std::shared_ptr<ASTNode>(new ASTNodeIdentifier(val))};
            }
        }
        }
    }

    assert(s.size() == 1);
    return {s.top()};
}

Result<std::shared_ptr<ASTNode>> Parser::ParserFunctions::_parse_operator(
    std::stack<std::shared_ptr<ASTNode>> & stack, char op) {
    if (stack.size() < 2) {
        return {ParseError("Not enough arguments.")};
    }
    auto rhs = stack.top();
    stack.pop();
    auto lhs = stack.top();
    stack.pop();

    switch (op) {
    case '+':
        return {std::shared_ptr<ASTNode>(new ASTNodeAdd(lhs, rhs))};
    case '-':
        return {std::shared_ptr<ASTNode>(new ASTNodeSub(lhs, rhs))};
    case '*':
        return {std::shared_ptr<ASTNode>(new ASTNodeMul(lhs, rhs))};
    case '/':
        return {std::shared_ptr<ASTNode>(new ASTNodeDiv(lhs, rhs))};
    case '>':
        return {std::shared_ptr<ASTNode>(new ASTNodeGT(lhs, rhs))};
    case '<':
        return {std::shared_ptr<ASTNode>(new ASTNodeLT(lhs, rhs))};
    case '=':
        return {std::shared_ptr<ASTNode>(new ASTNodeEQ(lhs, rhs))};
    }

    return {ParseError("Invalid operator.")}; // this shouldn't happen
}

Result<std::shared_ptr<ASTNode>>
Parser::ParserFunctions::_parse_if(std::list<Token::Token> & tokens) {
    auto cond = _parse(tokens);
    if (!cond.valid()) {
        return cond;
    }
    auto tb = _parse(tokens);
    if (!tb.valid()) {
        return tb;
    }
    auto fb = _parse(tokens);
    if (!fb.valid()) {
        return fb;
    }

    if (!_check_closing_parenthesis(tokens)) {
        return {ParseError("If statement missing closing parenthesis.")};
    }

    tokens.pop_front();

    return {std::shared_ptr<ASTNode>(
        new ASTNodeIf(cond.value(), tb.value(), fb.value()))};
}

Result<std::shared_ptr<ASTNode>>
Parser::ParserFunctions::_parse_cons(std::list<Token::Token> & tokens) {
    auto car = _parse(tokens);
    if (!car.valid()) {
        return car;
    }
    auto cdr = _parse(tokens);
    if (!cdr.valid()) {
        return cdr;
    }

    if (!_check_closing_parenthesis(tokens)) {
        return {ParseError("Cons missing closing parenthesis.")};
    }

    tokens.pop_front();

    return {
        std::shared_ptr<ASTNode>(new ASTNodeCons(car.value(), cdr.value()))};
}

Parser::Parser() : _builtin({"let", "letrec", "lambda", "if", "nil", "cons"}){};

Result<std::list<std::shared_ptr<ASTNode>>>
Parser::parse(std::list<Token::Token> tokens) const {
    std::list<std::shared_ptr<ASTNode>> result;
    while (!tokens.empty()) {
        auto parsed_node = ParserFunctions::_parse(tokens);
        if (!parsed_node.valid()) {
            return {parsed_node.error()};
        }
        result.push_back(parsed_node.value());
    }
    return result;
}

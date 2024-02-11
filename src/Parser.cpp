#include "Parser.h"
#include <cassert>
#include <stack>

std::unordered_map<std::string, Parser::ParserFunctions::ParseFunction>
    Parser::ParserFunctions::_parse_map{
        {"if", Parser::ParserFunctions::_parse_if}};

Result<std::shared_ptr<ASTNode>>
Parser::ParserFunctions::_parse(std::list<Token> & tokens) {
    assert(!tokens.empty());
    Token curr = tokens.front();
    tokens.pop_front();
    switch (curr.data.index()) {
    case 0: { // char
        switch (std::get<char>(curr.data)) {
        case '(':
            return _parse_ex(tokens);
        case ')':
            return {ParseError("Unpaired closing parenthesis.")};
        }
        }
    case 1: // int
        return {
            std::shared_ptr<ASTNode>(new ASTNodeInt(std::get<int>(curr.data)))};
    case 2: { // string
        std::string val = std::get<std::string>(curr.data);
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
Parser::ParserFunctions::_parse_ex(std::list<Token> & tokens) {
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
            case '+':
            case '-':
            case '*':
            case '/':
            case '>':
            case '<':
            case '=': {
                char op = std::get<char>(token.data);
                if (auto res = _parse_operator(s, op); !res.valid()) {
                    return res;
                } else {
                    s.push(res.value());
                }
            }
            }
            break;
        }
        case 1: // int
            s.push(std::shared_ptr<ASTNode>(
                new ASTNodeInt(std::get<int>(token.data))));
            break;
        case 2: { // string
            std::string val = std::get<std::string>(token.data);
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
Parser::ParserFunctions::_parse_if(std::list<Token> & tokens) {
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

    if (tokens.front().data.index() != 0 || std::get<char>(tokens.front().data) != ')') {
        return {ParseError("If statement missing closing parenthesis.")};
    }

    tokens.pop_front();

    return {std::shared_ptr<ASTNode>(
        new ASTNodeIf(cond.value(), tb.value(), fb.value()))};
}

Parser::Parser() : _builtin({"let", "letrec", "lambda", "if", "nil", "cons"}){};

Result<std::list<std::shared_ptr<ASTNode>>>
Parser::parse(std::list<Token> tokens) const {
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

#include "Tokenizer.h"
#include <fstream>

Result<std::list<Token::Token>> Tokenizer::tokenize(const std::string & filename) const {
    std::ifstream in(filename);
    if (!in.is_open() || in.bad() || in.fail()) {
        return {ParseError("Couldn't read from file: " + filename)};
    }
    std::list<Token::Token> result;

    while(!in.eof()) {
        if (isspace(in.peek())) {
            (void)in.get();
            continue;
        } else if (isdigit(in.peek())) {
            int i;
            in >> i;
            result.emplace_back(i);
            continue;
        }

        switch (in.peek()) {
        case ')':
        case '(':
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '>':
        case '=':
            result.emplace_back((char)in.get());
            break;

        default:
            if (auto str = _tokenize_string(in); !str.empty()) {
                result.emplace_back(str);
            }
        }
    }

    return {result};
}

std::string Tokenizer::_tokenize_string(std::ifstream & in) const {
    std::string res;
    while(in.good() && !isspace(in.peek()) && in.peek() != '(' && in.peek() != ')') {
        res += in.get();
    }
    return res;
}

#include "Tokenizer.h"
#include <fstream>

Result<std::list<Token::Token>> Tokenizer::tokenize(std::istream & in) const {
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
        case ';': {
            _skip_comment(in);
            break;
        }
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

std::string Tokenizer::_tokenize_string(std::istream & in) const {
    std::string res;
    while(in.good() && !isspace(in.peek()) && in.peek() != '(' && in.peek() != ')') {
        res += in.get();
    }
    return res;
}

void Tokenizer::_skip_comment(std::istream & is) const {
    std::string garbage;
    std::getline(is, garbage);
}
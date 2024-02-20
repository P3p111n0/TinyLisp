#include "StdinTokenizer.h"
#include <sstream>
#include <string>

Result<std::list<Token::Token>> StdinTokenizer::tokenize() const {
    std::string input;
    std::getline(std::cin, input);
    if (std::cin.bad()) {
        return {ParseError("Error while reading from stdin.")};
    }
    std::stringstream in(input);

    return Tokenizer::tokenize(in);
}

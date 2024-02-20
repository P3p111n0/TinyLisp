#include "FileTokenizer.h"
#include <fstream>

Result<std::list<Token::Token>> FileTokenizer::tokenize() const {
    std::ifstream in(_filename);
    if (!in.is_open() || in.bad() || in.fail()) {
        return {ParseError("Couldn't read from file: " + _filename)};
    }
    return Tokenizer::tokenize(in);
}

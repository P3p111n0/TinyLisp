#include "Tokenizer.h"
#include "Parser.h"
#include "Compiler.h"
#include "SECD.h"
#include <iostream>

int main(int argc, const char ** argv) {
    if (argc < 2) {
        std::cout << "Filename missing." << std::endl;
        return 1;
    }

    Tokenizer t;
    auto tokens = t.tokenize(argv[1]);
    if (!tokens.valid()) {
        std::cout << tokens.error() << std::endl;
        return 1;
    }

    Parser p;
    auto ast = p.parse(tokens.value());
    if (!ast.valid()) {
        std::cout << ast.error() << std::endl;
        return 1;
    }

    auto code = Compiler::compile(ast.value());
    if (!code.valid()) {
        std::cout << code.error() << std::endl;
        return 1;
    }

    SECD secd;
    auto ret = secd.run(code.value());
    if (ret.has_value()) {
        std::cout << ret.value() << std::endl;
        return 1;
    }

    return 0;
}
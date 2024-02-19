#include "Compiler.h"
#include "FileTokenizer.h"
#include "Parser.h"
#include "SECD.h"
#include "StdinTokenizer.h"
#include <cstring>
#include <iostream>

void print_help() {
    std::cout << "TinyLisp" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Usage: \n\tTinyLisp <filename> to execute a file." << std::endl;
    std::cout << "\tRun without arguments to use a simple REPL environment."
              << std::endl;
}

std::optional<Error> run(const FileTokenizer & t) {
    auto tokens = t.tokenize();
    if (!tokens.valid()) {
        return tokens.error();
    }

    Parser p;
    auto ast = p.parse(tokens.value());
    if (!ast.valid()) {
        return ast.error();
    }

    auto code = Compiler::compile(ast.value());
    if (!code.valid()) {
        return code.error();
    }

    SECD secd;
    auto ret = secd.run(code.value());
    if (ret.has_value()) {
        return ret;
    }

    return std::nullopt;
}

std::optional<Error> run(const StdinTokenizer & t) {
    while (!std::cin.eof()) {
        std::cout << ">>> ";
        auto tokens = t.tokenize();
        if (!tokens.valid()) {
            return tokens.error();
        }

        Parser p;
        auto ast = p.parse(tokens.value());
        if (!ast.valid()) {
            return ast.error();
        }

        auto code = Compiler::compile(ast.value());
        if (!code.valid()) {
            return code.error();
        }

        SECD secd;
        auto ret = secd.run(code.value());
        if (ret.has_value()) {
            return ret;
        }
    }

    return std::nullopt;
}

int main(int argc, const char ** argv) {
    if (argc == 1) {
        StdinTokenizer t;
        while (!std::cin.eof()) {
            if (auto result = run(t); result.has_value()) {
                std::cout << result.value() << std::endl;
            }
        }
    } else if (argc == 2) {
        if (!strcmp(argv[1], "-h")) {
            print_help();
            return 0;
        } else {
            FileTokenizer t(argv[1]);
            if (auto result = run(t); result.has_value()) {
                std::cout << result.value() << std::endl;
                return 1;
            }
        }
    } else {
        std::cout << "Invalid number of arguments." << std::endl;
        return 1;
    }
    return 0;
}
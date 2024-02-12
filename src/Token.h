#pragma once

#include <string>
#include <variant>

namespace Token {
    enum TokenIndex {
        Char,
        Int,
        String
    };
    using Token = std::variant<char, int, std::string>;
}

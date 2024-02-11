#pragma once

#include <string>
#include <variant>

struct Token {
    std::variant<char, int, std::string> data;
};

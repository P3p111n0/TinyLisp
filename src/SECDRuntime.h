#pragma once

#include <list>
#include <stack>
#include <variant>
#include <memory>

enum class ValueIndex {
    Nullptr_t,
    Bool,
    Int
};

using Value = std::variant<nullptr_t, bool, int>;

class SECDInstruction;

struct SECDRuntime {
    SECDRuntime() = default;
    std::stack<Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
};
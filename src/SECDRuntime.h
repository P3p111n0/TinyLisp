#pragma once

#include <list>
#include <memory>
#include <stack>
#include <variant>

namespace Value {
    enum ValueIndex { Nullptr_t, Bool, Int };

    using Value = std::variant<nullptr_t, bool, int>;
}

class SECDInstruction;

struct SECDRuntime {
    SECDRuntime() = default;
    std::stack<Value::Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
};
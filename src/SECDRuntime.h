#pragma once

#include <list>
#include <memory>
#include <stack>
#include <variant>
#include <iostream>

namespace Value {
enum ValueIndex { Nullptr_t, Bool, Int, ConsCell };

struct Cons;
using Value = std::variant<nullptr_t, bool, int, Cons>;
struct Cons {
    Cons(const Value & car_, const Value & cdr_)
        : car(std::make_shared<Value>(car_)),
          cdr(std::make_shared<Value>(cdr_)){};
    std::shared_ptr<Value> car;
    std::shared_ptr<Value> cdr;
};

std::ostream & operator<<(std::ostream & os, const Value & val) {
    switch(val.index()) {
    case ValueIndex::Int:
        return os << std::get<int>(val);
    case ValueIndex::Bool:
        return os << (std::get<bool>(val) ? "true" : "false");
    case ValueIndex::Nullptr_t:
        return os << "NIL";
    case ValueIndex::ConsCell: {
        Cons cell = std::get<Cons>(val);
        return os << "( " << *cell.car << " . " << *cell.cdr << " )";
    }
    }
}

} // namespace Value

class SECDInstruction;

struct SECDRuntime {
    SECDRuntime() = default;
    std::stack<Value::Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
};
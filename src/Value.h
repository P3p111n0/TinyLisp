#pragma once

#include <memory>
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

std::ostream & operator<<(std::ostream & os, const Value & val);

} // namespace Value
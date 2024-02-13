#pragma once

#include "Result.h"
#include <list>
#include <memory>
#include <stack>
#include <variant>
#include <iostream>
#include <unordered_map>

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

class Environment {
  public:
    Environment() = default;
    Result<Value::Value> find(const std::string &) const;
    void insert(const std::string &, const Value::Value &);
    Environment start_child();

  private:
    std::unordered_map<std::string, Value::Value> _env;
    Environment * _parent;
};

class SECDInstruction;

struct SECDRuntime {
    SECDRuntime() = default;
    std::stack<Value::Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
    Environment env;
};
#include "SECDRuntime.h"

std::ostream & Value::operator<<(std::ostream & os, const Value & val) {
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
    return os; // this shouldn't happen
}

Result<Value::Value> Environment::find(const std::string & name) const {
    if (_env.contains(name)) {
        return _env.at(name);
    }
    if (_parent) {
        return _parent->find(name);
    }
    return {RuntimeError("Unbound identifier: " + name)};
}

void Environment::insert(const std::string & name, const Value::Value & val) {
    _env[name] = val;
}

Environment Environment::start_child() {
    Environment child;
    child._parent = this;
    return child;
}
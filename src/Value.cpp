#include "Value.h"

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
    case ValueIndex::FunctionClosure:
        return os << "#<closure>#";
    }
    return os; // this shouldn't happen
}

bool Value::operator==(const Value & lhs, const Value & rhs) {
    if (lhs.index() != rhs.index()) {
        return false;
    }

    switch (lhs.index()) {
    case ValueIndex::Nullptr_t:
        return true; // nullptr == nullptr
    case ValueIndex::Int:
        return std::get<int>(lhs) == std::get<int>(rhs);
    case ValueIndex::Bool:
        return std::get<bool>(lhs) == std::get<bool>(rhs);
    case ValueIndex::ConsCell: {
        Cons cons_lhs = std::get<Cons>(lhs);
        Cons cons_rhs = std::get<Cons>(rhs);
        bool car = *cons_lhs.car == *cons_rhs.car;
        bool cdr = *cons_lhs.cdr == *cons_rhs.cdr;
        return car && cdr;
    }
    case ValueIndex::FunctionClosure:
        //TODO
        //don't know how to compare closures yet :/
        return false;
    default:
        return false;
    }
}
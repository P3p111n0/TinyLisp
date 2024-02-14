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
#include "SECDInstruction.h"

std::optional<Error> LDC::execute(SECDRuntime & runtime) const {
    runtime.stack.emplace(_val);
    return std::nullopt;
}

std::optional<Error> NIL::execute(SECDRuntime & runtime) const {
    runtime.stack.emplace(nullptr);
    return std::nullopt;
}

Result<ArithmeticInstruction::Args>
ArithmeticInstruction::_get_args(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 2) {
        return {RuntimeError("Not enough arguments.")};
    }
    auto rhs = runtime.stack.top();
    runtime.stack.pop();
    auto lhs = runtime.stack.top();
    runtime.stack.pop();

    if (rhs.index() != (size_t)ValueIndex::Int ||
        lhs.index() != (size_t)ValueIndex::Int) {
        return {RuntimeError("Invalid operand type.")};
    }

    return {{std::get<int>(lhs), std::get<int>(rhs)}};
}

std::optional<Error> ADD::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs + args.rhs);
    return std::nullopt;
}

std::optional<Error> SUB::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs - args.rhs);
    return std::nullopt;
}

std::optional<Error> MUL::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs * args.rhs);
    return std::nullopt;
}

std::optional<Error> DIV::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs / args.rhs);
    return std::nullopt;
}
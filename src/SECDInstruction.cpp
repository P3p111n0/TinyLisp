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
    if (!args.rhs) {
        return {RuntimeError("Division by zero.")};
    }
    runtime.stack.emplace(args.lhs / args.rhs);
    return std::nullopt;
}

std::optional<Error> GT::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs > args.rhs);
    return std::nullopt;
}

std::optional<Error> LT::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs < args.rhs);
    return std::nullopt;
}

std::optional<Error> EQ::execute(SECDRuntime & runtime) const {
    auto args_res = _get_args(runtime);
    if (!args_res.valid()) {
        return {args_res.error()};
    }
    auto args = args_res.value();
    runtime.stack.emplace(args.lhs == args.rhs);
    return std::nullopt;
}

std::optional<Error> InstructionGlob::execute(SECDRuntime & runtime) const {
    for (const auto & instruction : _inst) {
        auto ret = instruction->execute(runtime);
        if (ret.has_value()) {
            return ret.value();
        }
    }
    return std::nullopt;
}

std::optional<Error> SEL::_select_branch(SECDRuntime & runtime,
                                         bool cond_res) const {
    if (runtime.code.size() < 2) {
        return {RuntimeError("SEL - Branch missing.")};
    }
    if (!cond_res) {
        runtime.code.pop_front();
        return std::nullopt;
    }
    auto tb = runtime.code.front();
    runtime.code.pop_front();
    runtime.code.pop_front();
    runtime.code.push_front(tb);
    return std::nullopt;
}

std::optional<Error> SEL::execute(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 1) {
        return {RuntimeError("SEL - Not enough arguments.")};
    }
    Value cond = runtime.stack.top();
    runtime.stack.pop();
    switch (cond.index()) {
    case (size_t)ValueIndex::Bool: {
        return _select_branch(runtime, std::get<bool>(cond));
    }
    case (size_t)ValueIndex::Int: {
        return _select_branch(runtime, (bool)std::get<int>(cond));
    }
    default:
        return {RuntimeError("SEL - Invalid condition data type.")};
    }
}

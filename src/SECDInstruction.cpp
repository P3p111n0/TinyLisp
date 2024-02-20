#include "SECDInstruction.h"
#include "Value.h"

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

    if (rhs.index() != Value::ValueIndex::Int ||
        lhs.index() != Value::ValueIndex::Int) {
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
    if (runtime.stack.size() < 2) {
        return {RuntimeError("Not enough arguments.")};
    }
    auto rhs = runtime.stack.top();
    runtime.stack.pop();
    auto lhs = runtime.stack.top();
    runtime.stack.pop();

    if (lhs.index() == Value::ValueIndex::FunctionClosure ||
        rhs.index() == Value::ValueIndex::FunctionClosure) {
        return {RuntimeError("EQ - Cannot compare functions for equality.")};
    }

    runtime.stack.emplace(lhs == rhs);
    return std::nullopt;
}

std::optional<Error> InstructionGlob::execute(SECDRuntime & runtime) const {
    auto code = _inst;
    runtime.code.splice(runtime.code.begin(), code);
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
    Value::Value cond = runtime.stack.top();
    runtime.stack.pop();
    switch (cond.index()) {
    case Value::ValueIndex::Bool: {
        return _select_branch(runtime, std::get<bool>(cond));
    }
    case Value::ValueIndex::Int: {
        return _select_branch(runtime, (bool)std::get<int>(cond));
    }
    default:
        return {RuntimeError("SEL - Invalid condition data type.")};
    }
}

std::optional<Error> CONS::execute(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 2) {
        return {RuntimeError("CONS - Not enough arguments.")};
    }

    Value::Value car = runtime.stack.top();
    runtime.stack.pop();
    Value::Value cdr = runtime.stack.top();
    runtime.stack.pop();
    runtime.stack.push(Value::Cons(car, cdr));
    return std::nullopt;
}

std::optional<Error> CAR::execute(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 1) {
        return {RuntimeError("CAR - Not enough arguments")};
    }

    Value::Value cons = runtime.stack.top();
    runtime.stack.pop();
    if (cons.index() != Value::ValueIndex::ConsCell) {
        return {RuntimeError("CAR - Cons cell expected.")};
    }

    Value::Cons cell = std::get<Value::Cons>(cons);
    runtime.stack.emplace(*cell.car);
    return std::nullopt;
}

std::optional<Error> CDR::execute(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 1) {
        return {RuntimeError("CDR - Not enough arguments")};
    }

    Value::Value cons = runtime.stack.top();
    runtime.stack.pop();
    if (cons.index() != Value::ValueIndex::ConsCell) {
        return {RuntimeError("CDR - Cons cell expected.")};
    }

    Value::Cons cell = std::get<Value::Cons>(cons);
    runtime.stack.emplace(*cell.cdr);
    return std::nullopt;
}

std::optional<Error> LD::execute(SECDRuntime & runtime) const {
    auto val = runtime.env.get(_idx);
    if (!val.valid()) {
        return {val.error()};
    }
    runtime.stack.emplace(val.value());
    return std::nullopt;
}

std::optional<Error> LDF::execute(SECDRuntime & runtime) const {
    if (runtime.code.size() < 1) {
        return {RuntimeError("LDF - Code missing")};
    }
    Closure closure{runtime.code.front(), std::make_shared<RTEnv>(runtime.env)};
    runtime.code.pop_front();
    runtime.stack.emplace(closure);
    return std::nullopt;
}

std::list<Value::Value> AP::_cons_to_list(const Value::Cons & cons) const {
    std::list<Value::Value> res;
    res.emplace_back(*cons.car);
    Value::Value cdr = *cons.cdr;
    while (cdr.index() != Value::ValueIndex::Nullptr_t) {
        auto cell = std::get<Value::Cons>(cdr);
        res.emplace_back(*cell.car);
        cdr = *cell.cdr;
    }
    return res;
}

std::optional<Error> AP::execute(SECDRuntime & runtime) const {
    if (runtime.stack.size() < 2) {
        return {RuntimeError("AP - Not enough arguments.")};
    }
    auto closure_variant = runtime.stack.top(); runtime.stack.pop();
    if (closure_variant.index() != Value::ValueIndex::FunctionClosure) {
        return {RuntimeError("AP - Closure expected.")};
    }
    auto closure = std::get<Closure>(closure_variant);

    auto args = runtime.stack.top(); runtime.stack.pop();
    runtime.dump.emplace(DumpStruct{runtime.stack, runtime.code, runtime.env});
    runtime.stack = {}; runtime.code = {}; runtime.env = {};
    runtime.code = {closure.code}; runtime.env = *closure.env;
    runtime.env.add_to_current(closure); // enables recursion
    if (args.index() == Value::ValueIndex::ConsCell) {
        auto args_list = _cons_to_list(std::get<Value::Cons>(args));
        runtime.env.add(args_list);
    } else {
        runtime.env.add({args});
    }

    return std::nullopt;
}

std::optional<Error> RTN::execute(SECDRuntime & runtime) const {
    if (runtime.stack.empty()) {
        return {RuntimeError("RTN - No return value found")};
    }
    auto ret_val = runtime.stack.top(); runtime.stack.pop();
    if (runtime.dump.empty()) {
        return {RuntimeError("RTN - Dump is empty.")};
    }
    auto dump = runtime.dump.top(); runtime.dump.pop();
    runtime.stack = std::move(dump.stack);
    runtime.code = std::move(dump.code);
    runtime.env = std::move(dump.env);
    runtime.stack.emplace(ret_val);
    return std::nullopt;
}

std::optional<Error> DEFUN::execute(SECDRuntime & runtime) const {
    // this part is sketchy
    auto it = _fun.begin(); // skip LDF
    std::advance(it, 1);
    auto lambda_code = *it; // get InstGlob pointer
    Closure lambda_closure = {lambda_code, std::make_shared<RTEnv>(runtime.env)};
    //lambda_closure.env->add_to_current(lambda_closure); // expose lambda to itself, enables recursion
    runtime.env.add_to_current(lambda_closure); // put lambda in env, where it can get loaded from relevant scopes
    return std::nullopt;
}
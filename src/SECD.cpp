#include "SECD.h"

std::optional<Error>
SECD::run(std::list<std::shared_ptr<SECDInstruction>> code) {
    _runtime.reset();
    _runtime.code = std::move(code);

    while (!_runtime.code.empty()) {
        auto inst = _runtime.code.front(); _runtime.code.pop_front();
        std::optional<Error> ret = inst->execute(_runtime);
        if (ret.has_value()) {
            _runtime.reset();
            return {ret.value()};
        }
    }

    std::stack<Value::Value> print_stack;
    while (!_runtime.stack.empty()) {
        print_stack.push(_runtime.stack.top());
        _runtime.stack.pop();
    }

    while (!print_stack.empty()) {
        auto val = print_stack.top();
        print_stack.pop();
        std::cout << val << std::endl;
    }

    return std::nullopt;
}

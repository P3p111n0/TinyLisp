#include "Environment.h"
#include <cassert>

void CTEnv::add(const std::string & name) {
    size_t n = _env.size();
    _env[name] = n;
}

Result<IndexPair> CTEnv::get(const std::string & name) const {
    size_t i = 0;
    if (_env.contains(name)) {
        return {{i, _env.at(name)}};
    }
    std::shared_ptr<CTEnv> curr = _parent;
    while (curr) {
        i++;
        if (curr->_env.contains(name)) {
            return {{i, curr->_env.at(name)}};
        }
        curr = curr->_parent;
    }

    return {RuntimeError("Unbound identifier: " + name)};
}

std::shared_ptr<CTEnv> CTEnv::derive(std::shared_ptr<CTEnv> parent_ptr) {
    auto env = std::make_shared<CTEnv>();
    env->_parent = parent_ptr;
    return env;
}

void RTEnv::add(const std::list<Value::Value> & env) {
    _env.emplace_front(env);
}

void RTEnv::add_to_current(const Value::Value & val) {
    if (_env.empty()) {
        _env.push_front({val});
        return;
    }
    _env.front().push_back(val);
}

Result<Value::Value> RTEnv::get(const IndexPair & idx) const {
    size_t i = idx.first;
    size_t j = idx.second;

    if (_env.size() <= i) {
        return {RuntimeError("Identifier index out of range.")};
    }
    auto it = _env.begin();
    std::advance(it, i);

    std::list<Value::Value> env = *it;
    if (env.size() <= j) {
        return {RuntimeError("Identifier index out of range.")};
    }

    auto subenv_it = env.begin();
    std::advance(subenv_it, j);

    return {*subenv_it};
}

void RTEnv::pop() {
    assert(!_env.empty());
    _env.pop_front();
}
#pragma once

#include "Result.h"
#include "Value.h"
#include <memory>
#include <unordered_map>
#include <list>
#include <string>

using IndexPair = std::pair<size_t, size_t>;

class CTEnv {
  public:

    CTEnv() = default;
    void add(const std::string &);
    Result<IndexPair> get(const std::string &) const;
    static std::shared_ptr<CTEnv> derive(std::shared_ptr<CTEnv>);
  private:
    std::unordered_map<std::string, size_t> _env;
    std::shared_ptr<CTEnv> _parent;
};

class RTEnv {
  public:
    RTEnv() = default;
    void add(const std::list<Value::Value> &);
    void add_to_current(const Value::Value &);
    Result<Value::Value> get(const IndexPair &) const;
    void pop();
  private:
    std::list<std::list<Value::Value>> _env;
};
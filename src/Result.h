#pragma once

#include "Error.h"
#include <variant>

template<typename T>
class Result {
  public:
    Result(const Error & err) : _data(err), _valid(false) {};
    Result(const T & val) : _data(val), _valid(true);

    bool valid() const {
        return _valid;
    }

    const T & value() const {
        if (!_valid) {
            throw std::runtime_error("Trying to get a value from a non-valid result.");
        }
        return std::get<T>(_data);
    }

    const Error & error() const {
        if (_valid) {
            throw std::runtime_error("Trying to get an error from a valid result.");
        }
        return std::get<Error>(_data);
    }

  private:
    std::variant<Error, T> _data;
    bool _valid;
};

#pragma once

#include <string>
#include <iostream>

class Error {
  public:
    Error(std::string msg) : _msg(std::move(msg)) {}
    friend std::ostream & operator<<(std::ostream & os, const Error & err) {
        return os << err._msg;
    }
    const std::string & what() const {
        return _msg;
    }
  private:
    std::string _msg;
};

class ParseError : public Error {};

class RuntimeError : public Error {};

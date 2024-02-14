#pragma once

#include "Result.h"
#include "Value.h"
#include "Environment.h"
#include <list>
#include <memory>
#include <stack>
#include <iostream>

class SECDInstruction;

struct DumpStruct {
    std::stack<Value::Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
    RTEnv env;
};

struct SECDRuntime {
    SECDRuntime() = default;
    std::stack<Value::Value> stack;
    std::list<std::shared_ptr<SECDInstruction>> code;
    std::stack<DumpStruct> dump;
    RTEnv env;
};
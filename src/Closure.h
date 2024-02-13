#pragma once

#include <memory>
#include <list>

class SECDInstruction;
class RTEnv;

struct Closure {
    std::shared_ptr<SECDInstruction> code;
    std::shared_ptr<RTEnv> env;
};
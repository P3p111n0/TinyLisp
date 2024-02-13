#pragma once

#include "ASTNode.h"
#include "Environment.h"
#include "Result.h"
#include <list>
#include <memory>

class Compiler {
  public:
    static Result<std::list<std::shared_ptr<SECDInstruction>>>
    compile(const std::list<std::shared_ptr<ASTNode>> &);
};

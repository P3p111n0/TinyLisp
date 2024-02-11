#pragma once

#include "SECDInstruction.h"
#include "SECDRuntime.h"
#include <list>
#include <memory>

class SECD {
  public:
    SECD() = default;
    std::optional<Error> run(std::list<std::shared_ptr<SECDInstruction>>);
  private:
    SECDRuntime _runtime;
};
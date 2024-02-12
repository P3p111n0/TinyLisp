#pragma once

#include "SECDRuntime.h"
#include "Error.h"
#include "Result.h"
#include <optional>

class SECDInstruction {
  public:
    virtual ~SECDInstruction() = default;
    virtual std::optional<Error> execute(SECDRuntime &) const = 0;
};

class LDC : public SECDInstruction {
  public:
    explicit LDC(int val) : _val(val) {};
    std::optional<Error> execute(SECDRuntime &) const override;
    ~LDC() override = default;

  private:
    int _val;
};

class ArithmeticInstruction : public SECDInstruction {
  protected:
    struct Args {
        int lhs;
        int rhs;
    };
    Result<Args> _get_args(SECDRuntime &) const;
};

class ADD : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
    ~ADD() override = default;
};

class SUB : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class MUL : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class DIV : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class NIL : public SECDInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class GT : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class LT : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class EQ : public ArithmeticInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class InstructionGlob : public SECDInstruction {
  public:
    InstructionGlob(std::list<std::shared_ptr<SECDInstruction>> lst) : _inst(std::move(lst)) {};
    std::optional<Error> execute(SECDRuntime &) const override;
  private:
    std::list<std::shared_ptr<SECDInstruction>> _inst;
};

class SEL : public SECDInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
  private:
    std::optional<Error> _select_branch(SECDRuntime &, bool) const;
};

class JOIN : public SECDInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};

class CONS : public SECDInstruction {
  public:
    std::optional<Error> execute(SECDRuntime &) const override;
};
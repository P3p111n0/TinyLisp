#pragma once

#include "Environment.h"
#include "SECDInstruction.h"
#include <list>
#include <memory>

class ASTNode {
  public:
    virtual ~ASTNode() = default;
    virtual Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const = 0;
};

class ASTNodeInt : public ASTNode {
  public:
    ASTNodeInt(int val) : _val(val) {}
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;

  private:
    int _val;
};

class ASTNodeNull : public ASTNode {
  public:
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeOperator : public ASTNode {
  public:
    ASTNodeOperator(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : _rhs(rhs), _lhs(lhs){};

  protected:
    Result<std::list<std::shared_ptr<SECDInstruction>>> _compile_args(std::shared_ptr<CTEnv>) const;
    Result<std::list<std::shared_ptr<SECDInstruction>>>
    _compile_op(const std::shared_ptr<SECDInstruction> &, std::shared_ptr<CTEnv>) const;
    std::shared_ptr<ASTNode> _rhs;
    std::shared_ptr<ASTNode> _lhs;
};

class ASTNodeAdd : public ASTNodeOperator {
  public:
    ASTNodeAdd(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeSub : public ASTNodeOperator {
  public:
    ASTNodeSub(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeMul : public ASTNodeOperator {
  public:
    ASTNodeMul(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeDiv : public ASTNodeOperator {
  public:
    ASTNodeDiv(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeCons : public ASTNode {
  public:
    ASTNodeCons(std::shared_ptr<ASTNode> car, std::shared_ptr<ASTNode> cdr)
        : _car(car), _cdr(cdr){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;

  private:
    std::shared_ptr<ASTNode> _car;
    std::shared_ptr<ASTNode> _cdr;
};

class ASTNodeGT : public ASTNodeOperator {
  public:
    ASTNodeGT(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeLT : public ASTNodeOperator {
  public:
    ASTNodeLT(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeEQ : public ASTNodeOperator {
  public:
    ASTNodeEQ(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeIdentifier : public ASTNode {
  public:
    ASTNodeIdentifier(std::string val) : _val(std::move(val)){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;

  private:
    std::string _val;
};

class ASTNodeIf : public ASTNode {
  public:
    ASTNodeIf(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> tb,
              std::shared_ptr<ASTNode> fb)
        : _cond(cond), _tb(tb), _fb(fb){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;

  private:
    std::shared_ptr<ASTNode> _cond;
    std::shared_ptr<ASTNode> _tb;
    std::shared_ptr<ASTNode> _fb;
};

class ASTNodeConsOperator : public ASTNode {
  public:
    ASTNodeConsOperator(std::shared_ptr<ASTNode> cell) : _cons(cell){};

  protected:
    Result<std::list<std::shared_ptr<SECDInstruction>>>
    _compile(const std::shared_ptr<SECDInstruction> &, std::shared_ptr<CTEnv>) const;
    std::shared_ptr<ASTNode> _cons;
};

class ASTNodeCar : public ASTNodeConsOperator {
  public:
    ASTNodeCar(std::shared_ptr<ASTNode> cell) : ASTNodeConsOperator(cell){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};

class ASTNodeCdr : public ASTNodeConsOperator {
  public:
    ASTNodeCdr(std::shared_ptr<ASTNode> cell) : ASTNodeConsOperator(cell){};
    Result<std::list<std::shared_ptr<SECDInstruction>>>
        compile(std::shared_ptr<CTEnv>) const override;
};
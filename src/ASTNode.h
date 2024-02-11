#pragma once

#include "SECDInstruction.h"
#include <list>
#include <memory>

class ASTNode {
  public:
    virtual ~ASTNode() = default;
    virtual std::list<std::shared_ptr<SECDInstruction>> compile() const = 0;
};

class ASTNodeInt : public ASTNode {
  public:
    ASTNodeInt(int val) : _val(val) {}
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;

  private:
    int _val;
};

class ASTNodeNull : public ASTNode {
  public:
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeOperator : public ASTNode {
  public:
    ASTNodeOperator(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : _rhs(rhs), _lhs(lhs){};

  protected:
    std::list<std::shared_ptr<SECDInstruction>> _compile_args() const;

    std::shared_ptr<ASTNode> _rhs;
    std::shared_ptr<ASTNode> _lhs;
};

class ASTNodeAdd : public ASTNodeOperator {
  public:
    ASTNodeAdd(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeSub : public ASTNodeOperator {
  public:
    ASTNodeSub(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeMul : public ASTNodeOperator {
  public:
    ASTNodeMul(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeDiv : public ASTNodeOperator {
  public:
    ASTNodeDiv(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeCons : public ASTNode {
  public:
    ASTNodeCons(std::shared_ptr<ASTNode> car, std::shared_ptr<ASTNode> cdr)
        : _car(car), _cdr(cdr){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;

  private:
    std::shared_ptr<ASTNode> _car;
    std::shared_ptr<ASTNode> _cdr;
};

class ASTNodeGT : public ASTNodeOperator {
  public:
    ASTNodeGT(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeLT : public ASTNodeOperator {
  public:
    ASTNodeLT(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};

class ASTNodeEQ : public ASTNodeOperator {
  public:
    ASTNodeEQ(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
        : ASTNodeOperator(lhs, rhs){};
    std::list<std::shared_ptr<SECDInstruction>> compile() const override;
};
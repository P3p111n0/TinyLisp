#include "ASTNode.h"

std::list<std::shared_ptr<SECDInstruction>> ASTNodeInt::compile() const {
    return {std::shared_ptr<SECDInstruction>(new LDC(_val))};
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeNull::compile() const {
    return {std::shared_ptr<SECDInstruction>(new NIL())};
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeOperator::_compile_args() const {
    auto compiled_lhs = _lhs->compile();
    auto compiled_rhs = _rhs->compile();
    std::list<std::shared_ptr<SECDInstruction>> res;
    for (auto & val : compiled_lhs) {
        res.emplace_back(val);
    }
    for (auto & val : compiled_rhs) {
        res.emplace_back(val);
    }
    return res;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeAdd::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new ADD()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeSub::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new SUB()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeMul::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new MUL()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeDiv::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new DIV()));
    return result;
}

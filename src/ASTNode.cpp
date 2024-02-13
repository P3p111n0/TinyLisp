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
    res.splice(res.end(), compiled_lhs);
    res.splice(res.end(), compiled_rhs);

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

std::list<std::shared_ptr<SECDInstruction>> ASTNodeGT::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new GT()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeLT::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new LT()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeEQ::compile() const {
    auto result = _compile_args();
    result.emplace_back(std::shared_ptr<SECDInstruction>(new EQ()));
    return result;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeIf::compile() const {
    std::list<std::shared_ptr<SECDInstruction>> res;
    res.splice(res.end(), _cond->compile());
    res.emplace_back(new SEL());
    auto tb_code = _tb->compile();
    auto fb_code = _fb->compile();
//    tb_code.emplace_back(new JOIN());
//    fb_code.emplace_back(new JOIN());
    res.emplace_back(new InstructionGlob(tb_code));
    res.emplace_back(new InstructionGlob(fb_code));
    return res;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeIdentifier::compile() const {
    return {}; // TODO
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeCons::compile() const {
    std::list<std::shared_ptr<SECDInstruction>> res;
    res.splice(res.end(), _cdr->compile());
    res.splice(res.end(), _car->compile());
    res.emplace_back(new CONS());
    return res;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeCar::compile() const {
    std::list<std::shared_ptr<SECDInstruction>> res = _cons->compile();
    res.emplace_back(new CAR());
    return res;
}

std::list<std::shared_ptr<SECDInstruction>> ASTNodeCdr::compile() const {
    std::list<std::shared_ptr<SECDInstruction>> res = _cons->compile();
    res.emplace_back(new CDR());
    return res;
}
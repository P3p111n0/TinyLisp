#include "ASTNode.h"

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeInt::compile(std::shared_ptr<CTEnv>) const {
    return {{std::shared_ptr<SECDInstruction>(new LDC(_val))}};
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeNull::compile(std::shared_ptr<CTEnv>) const {
    return {{std::shared_ptr<SECDInstruction>(new NIL())}};
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeOperator::_compile_args(std::shared_ptr<CTEnv> env) const {
    auto compiled_lhs = _lhs->compile(env);
    if (!compiled_lhs.valid()) {
        return compiled_lhs;
    }
    auto compiled_rhs = _rhs->compile(env);
    if (!compiled_rhs.valid()) {
        return compiled_rhs;
    }

    std::list<std::shared_ptr<SECDInstruction>> res;
    res.splice(res.end(), compiled_lhs.value());
    res.splice(res.end(), compiled_rhs.value());

    return res;
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeOperator::_compile_op(const std::shared_ptr<SECDInstruction> & inst,
                             std::shared_ptr<CTEnv> env) const {
    auto result = _compile_args(env);
    if (!result.valid()) {
        return result;
    }
    auto code = result.value();
    code.emplace_back(inst);
    return code;
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeAdd::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new ADD()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeSub::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new SUB()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeMul::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new MUL()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeDiv::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new DIV()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeGT::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new GT()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeLT::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new LT()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeEQ::compile(std::shared_ptr<CTEnv> env) const {
    return _compile_op(std::shared_ptr<SECDInstruction>(new EQ()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeIf::compile(std::shared_ptr<CTEnv> env) const {
    std::list<std::shared_ptr<SECDInstruction>> res;
    auto cond_code = _cond->compile(env);
    if (!cond_code.valid()) {
        return cond_code;
    }
    res.splice(res.end(), cond_code.value());
    res.emplace_back(new SEL());
    auto tb_code = _tb->compile(env);
    if (!tb_code.valid()) {
        return tb_code;
    }
    auto fb_code = _fb->compile(env);
    if (!fb_code.valid()) {
        return fb_code;
    }
    //    tb_code.emplace_back(new JOIN());
    //    fb_code.emplace_back(new JOIN());
    res.emplace_back(new InstructionGlob(tb_code.value()));
    res.emplace_back(new InstructionGlob(fb_code.value()));
    return res;
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeIdentifier::compile(std::shared_ptr<CTEnv> env) const {
    auto idx = env->get(_val);
    if (!idx.valid()) {
        return {idx.error()};
    }
    return {{std::shared_ptr<SECDInstruction>(new LD(idx.value()))}};
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeCons::compile(std::shared_ptr<CTEnv> env) const {
    std::list<std::shared_ptr<SECDInstruction>> res;
    auto cdr_code = _cdr->compile(env);
    if (!cdr_code.valid()) {
        return cdr_code;
    }
    auto car_code = _car->compile(env);
    if (!car_code.valid()) {
        return car_code;
    }
    res.splice(res.end(), cdr_code.value());
    res.splice(res.end(), car_code.value());
    res.emplace_back(new CONS());
    return res;
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeConsOperator::_compile(const std::shared_ptr<SECDInstruction> & inst,
                              std::shared_ptr<CTEnv> env) const {
    auto cons_code = _cons->compile(env);
    if (!cons_code.valid()) {
        return cons_code;
    }
    auto res = cons_code.value();
    res.emplace_back(inst);
    return res;
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeCar::compile(std::shared_ptr<CTEnv> env) const {
    return _compile(std::shared_ptr<SECDInstruction>(new CAR()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>>
ASTNodeCdr::compile(std::shared_ptr<CTEnv> env) const {
    return _compile(std::shared_ptr<SECDInstruction>(new CDR()), env);
}

Result<std::list<std::shared_ptr<SECDInstruction>>> ASTNodeLambda::compile(std::shared_ptr<CTEnv> env) const {
    std::shared_ptr<CTEnv> child_env = CTEnv::derive(env);
    for (const auto & id : _args) {
        child_env->add(id);
    }
    std::list<std::shared_ptr<SECDInstruction>> body_code;
    for (const auto & node : _body) {
        auto code = node->compile(child_env);
        if (!code.valid()) {
            return code;
        }
        body_code.splice(body_code.end(), code.value());
    }
    body_code.emplace_back(new RTN());

    std::list<std::shared_ptr<SECDInstruction>> res;
    res.emplace_back(new LDF());
    res.emplace_back(new InstructionGlob(std::move(body_code)));
    return {res};
}

Result<std::list<std::shared_ptr<SECDInstruction>>> ASTNodeFunctionCall::compile(std::shared_ptr<CTEnv> env) const {
    std::list<std::shared_ptr<SECDInstruction>> res;
    res.emplace_back(new NIL());
    for (const auto & node : _args) {
        auto code = node->compile(env);
        if (!code.valid()) {
            return code;
        }
        res.splice(res.end(), code.value());
        res.emplace_back(new CONS());
    }
    auto fun_code = _fun->compile(env);
    if (!fun_code.valid()) {
        return fun_code;
    }
    res.splice(res.end(), fun_code.value());
    res.emplace_back(new AP());
    return {res};
}
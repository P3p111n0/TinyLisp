#include "Compiler.h"

Result<std::list<std::shared_ptr<SECDInstruction>>>
Compiler::compile(const std::list<std::shared_ptr<ASTNode>> & ast) {
    std::shared_ptr<CTEnv> env = std::make_shared<CTEnv>();
    std::list<std::shared_ptr<SECDInstruction>> res;

    for (const auto & node : ast) {
        auto code = node->compile(env);
        if (!code.valid()) {
            return code;
        }
        res.splice(res.end(), code.value());
    }

    return res;
}
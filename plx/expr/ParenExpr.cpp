#include <cassert>

#include <plx/vm/VM.hpp>
#include <plx/expr/ParenExpr.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    ParenExpr::ParenExpr(Object* expr)
        : _expr {expr}
    {}

    Object* ParenExpr::close(Triple* env) {
        return _expr->close(env);
    }

    void ParenExpr::eval(VM* vm) {
        vm->pushExpr(_expr);
    }

    void ParenExpr::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expr);
    }

    void ParenExpr::showOn(std::ostream& ostream) const {
        ostream << '(' <<_expr << ')';
    }

    TypeId ParenExpr::typeId() const {
        return TypeId::E_PARENEXPR;
    }

}

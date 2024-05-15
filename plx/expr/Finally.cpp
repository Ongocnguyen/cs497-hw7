#include <cassert>

#include <plx/expr/Finally.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>

namespace PLX {

    Finally::Finally(Object* expr)
        : _expr {expr}
    {}

    Object* Finally::close(Triple* env) {
        return new Finally(_expr->close(env));
    }

    void Finally::eval(VM* vm) {
        vm->pushExpr(_expr);
    }

    Object* Finally::expr() {
        return _expr;
    }

    List* Finally::freeVars(List* freeVars) {
        (void)freeVars;
        // TODO complete this
        assert(false);
    }

    void Finally::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expr);
    }

    bool Finally::match(Object* other, Triple*& bindings) {
        // TODO complete this
        (void)other;
        (void)bindings;
        assert(false);
    }

    void Finally::showOn(std::ostream& ostream) const {
        ostream << "Finally{" << _expr << "}";
    }

    TypeId Finally::typeId() const {
        return TypeId::C_FINALLY;
    }

}

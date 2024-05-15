#include <cassert>

#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Throw.hpp>
#include <plx/expr/ThrowContin.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Throw::Throw(Object* expr)
        : _expr {expr}
    {}

    Object* Throw::close(Triple* env) {
        return new Throw(_expr->close(env));
    }

    void Throw::eval(VM* vm) {
        vm->pushExpr(new ThrowContin());
        vm->pushExpr(_expr);
    }

    void Throw::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expr);
    }

    void Throw::showOn(std::ostream& ostream) const {
        ostream << "throw " << _expr;
    }

    TypeId Throw::typeId() const {
        return TypeId::E_THROW;
    }

}

#include <cassert>

#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Catch.hpp>
#include <plx/expr/Drop.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>

namespace PLX {

    Catch::Catch(Triple* rules)
        : _rules {rules}
    {}

    bool Catch::catchException(Object* exception, VM* vm) {
        Triple* bindings = vm->environment();
        Object* rhs;
        if (_rules->matchLocate(exception, rhs, bindings)) {
            vm->pushExpr(rhs, bindings);
            return true;
        }
        return false;
    }

    void Catch::eval(VM* vm) {
        (void)vm;
        // Do nothing. Don't even push anything.
    }

    List* Catch::freeVars(List* freeVars) {
        (void)freeVars;
        // TODO complete this
        assert(false);
    }

    void Catch::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_rules);
    }

    bool Catch::match(Object* other, Triple*& bindings) {
        // TODO complete this
        (void)other;
        (void)bindings;
        assert(false);
    }

    void Catch::showOn(std::ostream& ostream) const {
        ostream << "Catch{" << _rules << "}";
    }

    TypeId Catch::typeId() const {
        return TypeId::C_CATCH;
    }

}

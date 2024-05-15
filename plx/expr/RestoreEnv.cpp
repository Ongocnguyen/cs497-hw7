#include <cassert>

#include <plx/expr/RestoreEnv.hpp>

namespace PLX {

    RestoreEnv::RestoreEnv(Triple* savedEnv)
        : _savedEnv {savedEnv}
    {}

    void RestoreEnv::eval(VM* vm) {
        vm->setEnvironment(_savedEnv);
    }

    void RestoreEnv::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_savedEnv);
    }

    void RestoreEnv::pushContin(Triple* env, VM* vm) {
        Object* expr;
        bool peekRes = vm->peekExpr(expr);
        if (peekRes && expr->isA(TypeId::C_RESTOREENV)) {
            return;
        }
        RestoreEnv* restoreEnv = new RestoreEnv(env);
        vm->pushExpr(restoreEnv);
    }

    void RestoreEnv::showOn(std::ostream& ostream) const {
        ostream << "RestoreEnv{<hidden>}";
    }

    TypeId RestoreEnv::typeId() const {
        return TypeId::C_RESTOREENV;
    }

}

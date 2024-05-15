#include <cassert>

#include <plx/data/Triple.hpp>
#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Do.hpp>
#include <plx/expr/Drop.hpp>
#include <plx/expr/RestoreEnv.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Do::Do(List* exprs)
        : _exprs {exprs}
        , _reverseExprs {exprs->reverse()}
    {}

    Object* Do::close(Triple* env) {
        List* exprs = static_cast<List*>(_exprs->close(env));
        if (exprs->isEmpty()) {
            return GLOBALS->NilObject();
        }
        if (exprs->restAsList()->isEmpty()) {
            return exprs->first();
        }
        return exprs;
    }

    void Do::eval(VM* vm) {
        if (_reverseExprs->isEmpty()) {
            vm->pushObj(GLOBALS->NilObject());
        }
        else {
            RestoreEnv::pushContin(vm->environment(), vm);
            bool firstIter = true;
            Drop* drop = new Drop();
            List* exprs = _reverseExprs;
            while (!exprs->isEmpty()) {
                if (firstIter) {
                    firstIter = false;
                }
                else {
                    vm->pushExpr_dynamic(drop);
                }
                vm->pushExpr_dynamic(exprs->first());
                exprs = exprs->restAsList();
            }
        }
    }

    void Do::markChildren(std::vector<Object*>& objs) {
        // Push the List* member variables onto the objs vector
    objs.push_back(_exprs->first());
    objs.push_back(_exprs->restAsList());
    }

    void Do::showOn(std::ostream& ostream) const {
        ostream << "do";
        if (!_exprs->isEmpty()) {
            ostream << ' ';
        }
        _exprs->showOnWith(ostream, "", " ", " end");
    }

    TypeId Do::typeId() const {
        return TypeId::E_SEQ;
    }

}

#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Do.hpp>
#include <plx/expr/Drop.hpp>
#include <plx/expr/Let.hpp>
#include <plx/expr/LetIn.hpp>
#include <plx/expr/RestoreEnv.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    LetIn::LetIn(Triple* bindings, Object* body)
        : _let {new Let(bindings)}
        , _body {body}
    {}

    void LetIn::eval(VM* vm) {
        RestoreEnv::pushContin(vm->environment(), vm);
        vm->pushExpr_dynamic(_body);
        vm->pushExpr_dynamic(new Drop());
        vm->pushExpr_dynamic(_let);
    }

    void LetIn::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_let);
        objs.push_back(_body);
    }

    void LetIn::showOn(std::ostream& ostream) const {
        ostream << _let
                << " in "
                << _body;
    }

    TypeId LetIn::typeId() const {
        return TypeId::E_LETIN;
    }

}
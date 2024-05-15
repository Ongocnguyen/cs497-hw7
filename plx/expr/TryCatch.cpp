#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Catch.hpp>
#include <plx/expr/Drop.hpp>
#include <plx/expr/Finally.hpp>
#include <plx/expr/TryCatch.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    TryCatch::TryCatch(Object* expr, Triple* catchRules)
        : TryCatch{expr, catchRules, GLOBALS->NilObject()}
    {}

    TryCatch::TryCatch(Object* expr, Triple* catchRules, Object* finallyExpr)
        : _expr {expr}
        , _catchRules {catchRules}
        , _finallyExpr {finallyExpr}
        , _finally {new Finally(_finallyExpr)}
    {}

    Object* TryCatch::close(Triple* env) {
        return new TryCatch(_expr->close(env), _catchRules, _finallyExpr->close(env));
    }

    void TryCatch::eval(VM* vm) {
        if (_finallyExpr != GLOBALS->NilObject()) {
            vm->pushExpr(new Drop());
            vm->pushExpr(_finally);
        }
        vm->pushExpr(new Catch(_catchRules));
        vm->pushExpr(_expr);
    }

    void TryCatch::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expr);
        objs.push_back(_catchRules);
        objs.push_back(_finally);
    }

    void TryCatch::showOn(std::ostream& ostream) const {
        ostream << "try "
            << _expr;
        if (!_catchRules->isEmpty()) {
            ostream << " catch ";
            _catchRules->showOnWith(ostream, " | ");
        }
        if (_finallyExpr != GLOBALS->NilObject()) {
            ostream << " finally " << _finallyExpr;
        }
        ostream << " end";
    }

    TypeId TryCatch::typeId() const {
        return TypeId::E_TRYCATCH;
    }

}

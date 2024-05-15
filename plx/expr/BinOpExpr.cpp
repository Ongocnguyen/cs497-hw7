#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/expr/BinOpExpr.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    BinOpExpr::BinOpExpr(Object* lhs, Identifier* oper, Object* rhs)
        : _lhs {lhs}
        , _operator {oper}
        , _rhs {rhs}
        , _apply {new Apply(oper, List::create({lhs, rhs}))}
    {}

    Object* BinOpExpr::close(Triple* env) {
        Object* lhs = _lhs->close(env);
        Object* oper = _operator->close(env);
        Object* rhs = _rhs->close(env);
        return new Apply(oper, List::create({lhs, rhs}));
    }

    void BinOpExpr::eval(VM* vm) {
        vm->pushExpr(_apply);
    }

    void BinOpExpr::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_lhs);
        objs.push_back(_operator);
        objs.push_back(_rhs);
        objs.push_back(_apply);
    }

    void BinOpExpr::showOn(std::ostream& ostream) const {
        std::string opName {_operator->name()};
        if (opName == ".") {
            ostream << _lhs << opName << _rhs;
        }
        else {
            ostream << _lhs << ' ' << opName << ' ' << _rhs;
        }
    }

    TypeId BinOpExpr::typeId() const {
        return TypeId::E_BINOPEXPR;
    }

}

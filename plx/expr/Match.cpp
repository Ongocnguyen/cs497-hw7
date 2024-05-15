#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Match.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Match::Match(Object* expression, Triple* rules)
        : _expression {expression}
        , _rules {rules}
    {}

    class MatchContin : public Object {
    public:
        MatchContin(Triple* rules)
            : _rules {rules}
        {}
        void eval(VM* vm) override {
            Object* exprValue;
            assert(vm->popObj(exprValue));
            Triple* env = vm->environment();
            Object* value;
            if (!_rules->matchLocate(exprValue, value, env)) {
                throwException("Match::eval", "Argument mismatch", new Array({exprValue, this}));
            }
            vm->pushObj(value);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "MatchContin{" << _rules << "}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        Triple* _rules;
    };

    void Match::eval(VM* vm) {
        vm->pushExpr(new MatchContin(_rules));
        vm->pushExpr(_expression);
    }

    void Match::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expression);
        objs.push_back(_rules);
    }

    void Match::showOn(std::ostream& ostream) const {
        ostream << "match " << _expression << " in ";
        _rules->showOnWith(ostream, " | ");
        ostream << " end";
    }

    TypeId Match::typeId() const {
        return TypeId::E_MATCH;
    }

}
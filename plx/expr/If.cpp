#include <cassert>

#include <plx/literal/Boolean.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/If.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    If::If(Object* cond, Object* conseq, Object* alt)
        : _cond {cond}
        , _conseq {conseq}
        , _alt {alt}
    {}

    Object* If::close(Triple* env) {
        Object* cond = _cond->close(env);
        if (cond->isA(TypeId::L_BOOLEAN)) {
            Boolean* b = static_cast<Boolean*>(cond);
            return b->boolValue() ? _conseq->close(env) : _alt->close(env);
        }
        Object* conseq = _conseq->close(env);
        Object* alt = _alt->close(env);
        return new If(cond, conseq, alt);
    }

    class IfContin : public Object {
    public:
        IfContin(Object* conseq, Object* alt)
            : _conseq {conseq}
            , _alt {alt}
        {}
        void eval(VM* vm) override {
            Object* condVal;
            assert(vm->popObj(condVal));
            vm->pushExpr(condVal->boolValue() ? _conseq : _alt);
        }
        void markChildren(std::vector<Object*>& objs) override {
            objs.push_back(_conseq);
            objs.push_back(_alt);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "IfContin{}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        Object* _conseq;
        Object* _alt;
    };

    void If::eval(VM* vm) {
        vm->pushExpr(new IfContin(_conseq, _alt));
        vm->pushExpr(_cond);
    }

    void If::markChildren(std::vector<Object*>& objs) {
         objs.push_back(_cond);
    objs.push_back(_conseq);
    objs.push_back(_alt);
    }

    void If::showOn(std::ostream& ostream) const {
        ostream << "if "
                << _cond
                << " then "
                << _conseq
                << " else "
                << _alt;
    }

    TypeId If::typeId() const {
        return TypeId::E_IF;
    }

}

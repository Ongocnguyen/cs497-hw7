#include <cassert>
#include <vector>

#include <plx/data/List.hpp>
#include <plx/data/Method.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/object/Object.hpp>

namespace PLX {

    Method::Method(Object* lhs, Identifier* rhs)
        : _lhs {lhs}
        , _rhs {rhs}
    {}

    class MethodContin : public Object {
    public:
        MethodContin(List* arguments)
            : _arguments {arguments}
        {}
        void eval(VM* vm) override {
            Object* rhsVal;
            assert(vm->popObj(rhsVal));
            Apply* app = new Apply(rhsVal, _arguments);
            vm->pushExpr(app);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "MethodContin{" << _arguments << "}";
        }
        TypeId typeId() const override {
            return TypeId::C_METHOD;
        }
    private:
        List* _arguments;
    };

    void Method::apply(VM* vm, List* arguments) {
        List* arguments1 = new List(_lhs, arguments);
        vm->pushExpr(new MethodContin(arguments1));
        vm->pushExpr(_rhs);
    }

    // Object* Method::close(Triple* env) {
    //     return new Method(_lhs->close(env), _rhs->close(env));
    // }

    List* Method::freeVars(List* freeVars) {
        return _lhs->freeVars(_rhs->freeVars(freeVars));
    }

    void Method::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_lhs);
        objs.push_back(_rhs);
    }

    void Method::showOn(std::ostream& ostream) const {
        ostream << _lhs << '.' << _rhs;
    }

    TypeId Method::typeId() const {
        return TypeId::D_METHOD;
    }

}

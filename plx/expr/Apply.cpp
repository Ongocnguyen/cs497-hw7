#include <cassert>

#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Apply::Apply(Object* abstractionObj, List* arguments)
        : _abstractionObj {abstractionObj}
        , _arguments {arguments}
    {}

    class ApplyContin : public Object {
    public:
        ApplyContin() {}
        void eval(VM* vm) override {
            Object* argumentsObj;
            assert(vm->popObj(argumentsObj));
            assert(argumentsObj->isA(TypeId::D_LIST));
            List* arguments = static_cast<List*>(argumentsObj);

            Object* abstractionValue;
            assert(vm->popObj(abstractionValue));

            abstractionValue->apply(vm, arguments);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "ApplyContin{}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    };

    Object* Apply::close(Triple* env) {
        Object* abstr = _abstractionObj->close(env);
        List* argList = static_cast<List*>(_arguments->close(env));
        return new Apply(abstr, argList);
    }

    void Apply::eval(VM* vm) {
        vm->pushExpr(new ApplyContin());
        vm->pushExpr(_arguments);
        vm->pushExpr(_abstractionObj);
    }

    void Apply::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_abstractionObj);
        objs.push_back(_arguments);
    }

    void Apply::showOn(std::ostream& ostream) const {
        switch (_abstractionObj->typeId()) {
            case TypeId::D_CLOSURE:
            case TypeId::E_FUNCTION:
            case TypeId::E_NAMEDFUNCTION:
                ostream << '(' << _abstractionObj << ')';
                break;
            default:
                ostream << _abstractionObj;
                break;
        }
        _arguments->showOnWith(ostream, "(", ", ", ")");
    }

    TypeId Apply::typeId() const {
        return TypeId::E_APPLY;
    }

}

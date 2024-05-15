#include <cassert>
#include <vector>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Function.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Closure::Closure(Function* function, Triple* lexicalEnvironment)
        : _function {function}
        , _lexicalEnvironment {lexicalEnvironment}
    {}

    class ClosureApplyContin : public Object {
    public:
        ClosureApplyContin(Function* function, Triple* lexicalEnvironment)
            : _function {function}
            , _lexicalEnvironment {lexicalEnvironment}
        {}
        void eval(VM* vm) override {
            Object* argsObj;
            assert(vm->popObj(argsObj));
            assert(argsObj->isA(TypeId::D_LIST));
            List* argValues = static_cast<List*>(argsObj);
            Triple* newEnvironment = _lexicalEnvironment;
            Object* body = _function->matchArgumentsToParameters(argValues, newEnvironment);
            // N.B.: This is where tail call elimination happens.
            // The 'body' value can be a function call, but that function call
            // doesn't happen inside this call to eval(). It's pushed onto the
            // expression stack, and the VM will evaluate it later, after this
            // call to the eval() function returns.
            vm->pushExpr(body, newEnvironment);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "ClosureApplyContin{" << _function << ", <hidden>}";
        }
        TypeId typeId() const override {
            return TypeId::C_APPLY;
        }
    private:
        Function* _function;
        Triple* _lexicalEnvironment;
    };

    void Closure::apply(VM* vm, List* arguments) {
        vm->pushExpr(new ClosureApplyContin(_function, _lexicalEnvironment));
        vm->pushObj(arguments);
    }

    void Closure::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_function);
        objs.push_back(_lexicalEnvironment);
    }

    void Closure::showOn(std::ostream& ostream) const {
        ostream << _function;
    }

    TypeId Closure::typeId() const {
        return TypeId::D_CLOSURE;
    }

}
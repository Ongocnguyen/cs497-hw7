#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Function.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Function::Function()
        : _parameters {GLOBALS->EmptyList()}
        , _body {GLOBALS->NilObject()}
        , _next {this}
    {}

    Function::Function(List* parameters, Object* body)
        : _parameters {parameters}
        , _body {body}
        , _next {GLOBALS->EmptyFunction()}
    {}

    class FunctionApplyContin : public Object {
    public:
        FunctionApplyContin(Function* function)
            : _function {function}
        {}
        // bool matchArgumentsToParameters(List* parameters, List* arguments, Triple*& env) {
        //     if (parameters->match(arguments, env)) {
        //     }
        //     return false;
        // }
        void eval(VM* vm) {
            Object* argsObj;
            assert(vm->popObj(argsObj));
            assert(argsObj->isA(TypeId::D_LIST));
            List* arguments = static_cast<List*>(argsObj);
            // try each rule
            Function* fun = _function;
            Triple* env = vm->environment();
            while (!fun->isEmpty()) {
                List* parameters = fun->_parameters;
                if (parameters->match(arguments, env)) {
                    Object* closedBody = fun->_body->close(env);
                    vm->pushExpr(closedBody);
                    return;
                }
                fun = fun->_next;
            }
            throwException("Function", "Argument mismatch", new Array({arguments, this}));
        }
    private:
        Function* _function;
    };

    void Function::apply(VM* vm, List* arguments) {
        vm->pushExpr(new FunctionApplyContin(this));
        vm->pushExpr(arguments);
    }

    Object* Function::close(Triple* env) {
        List* params = _parameters;
        while (!params->isEmpty()) {
            Object* param = params->first();
            env = new Triple(param, param, env);
            params = params->restAsList();
        }
        std::cerr << "Function::close closing body " << _body << " :: " << _body->typeName() << "\n";
        Object* closedBody = _body->close(env);
        if (!this->_next->isEmpty()) {
            Function* nextRule = static_cast<Function*>(_next->close(env));
            this->_next = nextRule;
        }
        return new Function(_parameters, closedBody);
    }

    void Function::eval(VM* vm) {
        vm->pushObj(new Closure(this, vm->environment()));
    }

    bool Function::isEmpty() const {
        return this == GLOBALS->EmptyFunction();
    }

     void Function::markChildren(std::vector<Object*>& objs) {
        if (!_parameters->isEmpty()) {
            objs.push_back(_parameters);
        }
        objs.push_back(_body);
        if (!_next->isEmpty()) {
            objs.push_back(_next);
        }
     }

    Object* Function::matchArgumentsToParameters(List* arguments, Triple*& lexicalEnvironment) {
        Function* rule = this;
        while (!rule->isEmpty()) {
            Triple* tempEnv = lexicalEnvironment;
            if (rule->_parameters->match(arguments, tempEnv)) {
                lexicalEnvironment = tempEnv;
                return rule->_body;
            }
            rule = rule->_next;
        }
        throwException("Function", "Argument mismatch", new Array({arguments, this}));
        return this;  // keep the compiler happy
    }

    void Function::setNext(Function* next) {
        _next = next;
    }

    void Function::showOn(std::ostream& ostream) const {
        ostream << "fun ";
        showRulesOn(ostream);
    }

    void Function::showRulesOn(std::ostream& ostream) const {
        const Function* functionRule = this;
        bool firstIter = true;
        while (!functionRule->isEmpty()) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                ostream << " | ";
            }
            functionRule->_parameters->showOnWith(ostream, "(", ", ", ")");
            ostream << " = " << functionRule->_body;
            functionRule = functionRule->_next;
        }
    }

    TypeId Function::typeId() const {
        return TypeId::E_FUNCTION;
    }

}
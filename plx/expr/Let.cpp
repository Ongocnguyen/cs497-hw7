#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/Let.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Let::Let(Triple* bindings)
        : _bindings {bindings}
    {}

    void prebindFreeVars_vm(Object* lhs, VM* vm) {
        List* freeVars = lhs->freeVars(GLOBALS->EmptyList());
        while (!freeVars->isEmpty()) {
            assert(freeVars->first()->isA(TypeId::E_IDENTIFIER));
            Identifier* ident = static_cast<Identifier*>(freeVars->first());
            vm->bind(ident, GLOBALS->NilObject());
            freeVars = freeVars->restAsList();
        }
    }

    void matchLhsToRhsVal(Object* lhs, Object* rhsVal, VM* vm) {
        Triple* newBindings = GLOBALS->EmptyTriple();
        if (!lhs->match(rhsVal, newBindings)) {
            throwException("Let", "Match failure", new Array({lhs, rhsVal}));
        }
        // for each binding in the LHS, re-bind the value in the env
        while (!newBindings->isEmpty()) {
            assert(newBindings->key()->isA(TypeId::E_IDENTIFIER));
            Identifier* ident = static_cast<Identifier*>(newBindings->key());
            Object* newValue = newBindings->value();
            vm->rebind(ident, newValue);
            newBindings = newBindings->next();
        }
    }

    class LetContin : public Object {
    public:
        LetContin(Triple* bindings)
            : _bindings {bindings}
         {}
        void eval(VM* vm) override {
            Object* rhsVal;
            assert(vm->popObj(rhsVal));
            Object* lhs = _bindings->key();
            matchLhsToRhsVal(lhs, rhsVal, vm);
            // do the next binding
            _bindings = _bindings->next();
            if (_bindings->isEmpty()) {
                vm->pushObj(GLOBALS->NilObject());
            }
            else {
                Object* lhs = _bindings->key();
                prebindFreeVars_vm(lhs, vm);
                vm->pushExpr(this);
                vm->pushExpr(_bindings->value());
            }
        }
        void markChildren(std::vector<Object*>& objs) override {
            objs.push_back(_bindings);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "LetContin{" << _bindings << "}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        Triple* _bindings;
    };

    void Let::eval(VM* vm) {
        if (_bindings->isEmpty()) {
            vm->pushObj(GLOBALS->NilObject());
        }
        else {
            Object* lhs = _bindings->key();
            prebindFreeVars_vm(lhs, vm);
            LetContin* letContin = new LetContin(_bindings);
            vm->pushExpr(letContin);
            vm->pushExpr(_bindings->value());
        }
    }

    void Let::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_bindings);
    }

    void Let::showOn(std::ostream& ostream) const {
        ostream << "let " << _bindings;
    }

    TypeId Let::typeId() const {
        return TypeId::E_LET;
    }

}
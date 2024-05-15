#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/expr/Catch.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/prim/DefinePrims.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    VM::VM()
        : _environment {GLOBALS->EmptyTriple()}
    {
        definePrims(this);
    }

    void VM::clearStacks() {
        this->_ostack.clear();
        this->_estack.clear();
        this->_vstack.clear();
    }

    Triple* VM::environment() const {
        return _environment;
    }

    Triple* VM::bind(Identifier* ident, Object* value) {
        _environment = new Triple(ident, value, _environment);
        return _environment;
    }

    // This function is NOT a drop-in replacement for the Evaluator::eval()
    // function. This is a convenience function to be used only in unit tests.
    Object* VM::evalExpr(Object* expr) {
        pushExpr(expr);
        return run();
    }

    Object* VM::lookup(Identifier* identifier) const {
        Object* value;
        if (!_environment->lookup(identifier, value)) {
            throwException("VM", "Unbound identifier", identifier);
        }
        return value;
    }

    void VM::markChildren(std::vector<Object*>& objs) {
        for (Object* obj : _ostack) {
            objs.push_back(obj);
        }
        for (Object* expr : _estack) {
            objs.push_back(expr);
        }
        for (Object* env : _vstack) {
            objs.push_back(env);
        }
        objs.push_back(_environment);
    }

    void VM::pushObj(Object* obj) {
        _ostack.push_back(obj);
    }

    bool VM::popObj(Object*& obj) {
        if (_ostack.empty()) {
            return false;
        }
        obj = _ostack.back();
        _ostack.pop_back();
        return true;
    }

    void VM::pushExpr(Object* expr) {
        _estack.push_back(expr);
        _vstack.push_back(_environment);
    }

    void VM::pushExpr(Object* expr, Triple* env) {
        _estack.push_back(expr);
        _vstack.push_back(env);
    }

    void VM::pushExpr_dynamic(Object* expr) {
        pushExpr(expr, GLOBALS->DynamicEnvironment());
    }

    bool VM::peekExpr(Object*& expr) {
        if (_estack.empty()) {
            return false;
        }
        expr = _estack.back();
        return true;
    }

    bool VM::popExpr(Object*& expr) {
        if (_estack.empty()) {
            return false;
        }
        expr = _estack.back();
        _estack.pop_back();
        Object* lexEnv = _vstack.back();
        _vstack.pop_back();
        if (lexEnv != GLOBALS->DynamicEnvironment()) {
            _environment = static_cast<Triple*>(lexEnv);
        }
        return true;
    }

    bool VM::rebind(Identifier* key, Object* value) {
        Triple* binding = _environment->locate(key);
        if (binding->isEmpty()) {
            return false;
        }
        binding->setValue(value);
        return false;
    }

    extern bool vmInterrupt;

    Object* VM::run() {
        while (step()) {
            if (_isGcNeeded) {
                GLOBALS->Gc()->collect();
            }
            if (vmInterrupt) {
                clearStacks();
                pushObj(Symbol::create("Interrupt"));
                vmInterrupt = false;
            }
        }
        Object* finalValue;
        if (!popObj(finalValue)) {
            throwException("VM", "Object stack is empty", this);
        }
        return finalValue;
    }

    void VM::setEnvironment(Triple* environment) {
        _environment = environment;
    }

    void VM::setGcNeeded() {
        _isGcNeeded = true;
    }

    void VM::setTrace(bool traceStep) {
        _traceStep = traceStep;
    }

    void VM::showStack(const std::vector<Object*>& stack) const {
        std::clog << '[';
        bool firstIter = true;
        for (int n=stack.size()-1; n>=0; n--) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                std::clog << ", ";
            }
            std::clog << stack[n] ;
        }
        std::clog << ']';
    }

    void VM::showOn(std::ostream& ostream) const {
        ostream << "VM{Ostack=[";
        showStack(_ostack);
        ostream << "], Estack=[";
        // show the estack
        showStack(_estack);
        ostream << "]}";
    }

    bool VM::step() {
        static GC* gc = GLOBALS->Gc();
        Object* expr;
        if (!popExpr(expr)) {
            return false;
        }
        if (_traceStep) {
            std::clog << "VM::step expr=" << expr << ", ostack=";
            showStack(_ostack);
            std::clog << ", estack=";
            showStack(_estack);
            std::clog << "\n";
        }
        expr->eval(this);
        if (gc->isGcNeeded()) {
            gc->collect();
        }
        return true;
    }

    TypeId VM::typeId() const {
        return TypeId::T_VM;
    }

}

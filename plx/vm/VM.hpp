#pragma once

#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class Identifier;
    class List;
    class Triple;

    class VM : public Object {
    public:
        VM();

        // Unique functions ------------------------------------------------

        Triple* bind(Identifier* ident, Object* value);
        Object* lookup(Identifier* ident) const;
        Triple* environment() const;
        void setEnvironment(Triple* environment);
        bool rebind(Identifier* key, Object* value);

        void pushObj(Object* obj);
        bool popObj(Object*& obj);

        void pushExpr(Object* expr);
        void pushExpr(Object* expr, Triple* env);
        void pushExpr_dynamic(Object* expr);
        bool peekExpr(Object*& expr);  // used by RestoreEnv continuation
        bool popExpr(Object*& expr);

        void clearStacks();

        bool step();
        Object* run();
        Object* evalExpr(Object* expr);

        void setGcNeeded();
        void setTrace(bool traceStep);
        void showStack(const std::vector<Object*>& stack) const;

        // Overridden functions --------------------------------------------

        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        std::vector<Object*> _ostack;
        std::vector<Object*> _estack;
        std::vector<Object*> _vstack;
        Triple* _environment;
        bool _isGcNeeded = false;
        bool _traceStep = false;
    };

}

#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class List;

    class Function : public Object {
    public:
        Function();
        Function(List* parameters, Object* body);

        // Unique functions ------------------------------------------------

        bool isEmpty() const;
        Object* matchArgumentsToParameters(List* arguments, Triple*& lexicalEnvironment);
        void setNext(Function* next);

        void showRulesOn(std::ostream& ostream) const;

        // Overridden functions --------------------------------------------

        void apply(VM* vm, List* arguments) override;
        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

        friend class NamedFunction;
        friend class FunctionApplyContin;

    private:
        List* _parameters;
        Object* _body;
        Function* _next;
    };

}

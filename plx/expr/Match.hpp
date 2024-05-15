#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Triple;

    class Match : public Object {
    public:
        Match(Object* expression, Triple* rules);

        // Unique functions ------------------------------------------------

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

        friend class NamedFunction;

    private:
        Object* _expression;
        Triple* _rules;
    };

}

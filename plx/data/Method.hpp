#pragma once

#include <list>
#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class Identifier;
    class List;
    class VM;

    class Method : public Object {
    public:
        Method(Object* lhs, Identifier* rhs);

        // Overridden functions --------------------------------------------

        void apply(VM* vm, List* arguments) override;
        // Object* close(Triple* env) override;
        List* freeVars(List* freeVars) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _lhs;
        Identifier* _rhs;
    };

}

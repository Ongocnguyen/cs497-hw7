#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Do;
    class Let;
    class Triple;
    class VM;

    class LetIn : public Object {
    public:
        LetIn(Triple* bindings, Object* body);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Let* _let;
        Object* _body;
    };

}

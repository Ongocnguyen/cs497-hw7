#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Function;
    class Identifier;
    class Let;

    class NamedFunction : public Object {
    public:
        NamedFunction(Identifier* name, Function* function);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        Object* close(Triple* env) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Identifier* _name;
        Function* _function;
        Let* _let;
    };

}

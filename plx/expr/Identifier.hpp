#pragma once

#include <plx/object/Symbolic.hpp>

namespace PLX {

    class Triple;
    class VM;

    class Identifier : public Symbolic {
    public:
        static Identifier* create(const std::string& name);

        // Unique functions ------------------------------------------------

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        List* freeVars(List* freeVars) override;
        bool match(Object* other, Triple*& bindings) override;
        TypeId typeId() const override;

    private:
        Identifier(const std::string& name);


    };

}
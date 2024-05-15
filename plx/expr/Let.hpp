#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Triple;
    class VM;

    class Let : public Object {
    public:
        Let(Triple* bindings);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Triple* _bindings;
    };

}

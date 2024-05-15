#pragma once

#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    typedef void (*Primitive)(VM* vm, List* arguments);

    class PrimitiveFunction : public Object {
    public:
        PrimitiveFunction(const std::string& name, Primitive primitive);

        // Unique functions ------------------------------------------------

        std::string name();
    
        // Overridden functions --------------------------------------------

        void apply(VM* vm, List* arguments) override final;
        bool hashCode(HashCode& hashCode) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        std::string _name;
        Primitive _primitive;
    };

}

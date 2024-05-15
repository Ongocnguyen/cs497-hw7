#pragma once

#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Nil : public Object {
    public:
        Nil();

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        void displayOn(std::ostream& ostream) const override;
        bool hashCode(HashCode& hashCode) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
    };

}

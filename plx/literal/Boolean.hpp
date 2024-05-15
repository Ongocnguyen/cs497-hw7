#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Boolean : public Object {
    public:
        Boolean(bool value);

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        bool equals(const Object* other) const override;
        bool hashCode(HashCode& hashCode) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        bool _value;

    };

}

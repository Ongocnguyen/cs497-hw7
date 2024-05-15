#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Integer : public Object {
    public:
        Integer(int value);

        // Unique functions ------------------------------------------------

        int value() const;

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        bool equals(const Object* other) const override;
        bool hashCode(HashCode& hashCode) override;
        bool plus(Object* other, Object*& value) override;
        bool minus(Object* other, Object*& value) override;
        bool times(Object* other, Object*& value) override;
        bool divide(Object* other, Object*& value) override;
        bool percent(Object* other, Object*& value) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        int _value;

    };

}

#include <plx/literal/Integer.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Integer::Integer(int value)
        : _value {value}
    {}

    bool Integer::boolValue() const {
        return _value != 0;
    }

    bool Integer::equals(const Object* other) const {
        if (other->isA(TypeId::L_INTEGER)) {
            const Integer* otherInteger = static_cast<const Integer*>(other);
            return _value == otherInteger->_value;
        }
        return false;
    }

    bool Integer::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        static const std::hash<int> intHasher;
        hashCode = typeHashCode ^ (intHasher(_value) << 1);
        return true;
    }

    bool Integer::plus(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        value = new Integer(_value + otherInt->_value);
        return true;
    }

    bool Integer::minus(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        value = new Integer(_value - otherInt->_value);
        return true;
    }

    bool Integer::times(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        value = new Integer(_value * otherInt->_value);
        return true;
    }

    bool Integer::divide(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        value = new Integer(_value / otherInt->_value);
        return true;
    }

    bool Integer::percent(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        value = new Integer(_value % otherInt->_value);
        return true;
    }

    void Integer::showOn(std::ostream& ostream) const {
        ostream << _value;
    }

    TypeId Integer::typeId() const {
        return TypeId::L_INTEGER;
    }

    int Integer::value() const {
        return _value;
    }

}

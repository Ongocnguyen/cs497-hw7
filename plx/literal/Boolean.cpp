#include <plx/literal/Boolean.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Boolean::Boolean(bool value)
        : _value {value}
    {}

    bool Boolean::boolValue() const {
        return _value;
    }

    bool Boolean::equals(const Object* other) const {
        if (other->isA(TypeId::L_BOOLEAN)) {
            const Boolean* otherBoolean = static_cast<const Boolean*>(other);
            return _value == otherBoolean->_value;
        }
        return false;
    }

    bool Boolean::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        static const std::hash<bool> boolHasher;
        static const size_t trueHash = typeHashCode ^ (boolHasher(true) << 1);
        static const size_t falseHash = typeHashCode ^ (boolHasher(false) << 1);
        hashCode = _value ? trueHash : falseHash;
        return true;
    }

    void Boolean::showOn(std::ostream& ostream) const {
        ostream << (_value ? "true" : "false");
    }

    TypeId Boolean::typeId() const {
        return TypeId::L_BOOLEAN;
    }

}

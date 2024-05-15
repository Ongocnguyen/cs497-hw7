#include <plx/literal/Nil.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Nil::Nil() {}

    bool Nil::boolValue() const {
        return false;
    }

    void Nil::displayOn(std::ostream& ostream) const {
        (void)ostream;
    }

    bool Nil::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        hashCode = typeHashCode;
        return true;
    }

    void Nil::showOn(std::ostream& ostream) const {
        ostream << "nil";
    }

    TypeId Nil::typeId() const {
        return TypeId::L_NIL;
    }

}

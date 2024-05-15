#include <plx/literal/String.hpp>
#include <plx/object/Symbolic.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    static HashCode _calculateHashCode(const std::string& name) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(TypeId::L_SYMBOL));
        static const std::hash<std::string> stringHasher;
        return typeHashCode ^ (stringHasher(name) << 1);
    }

    Symbolic::Symbolic(const std::string& name)
        : _name {name}
        , _hashCode {_calculateHashCode(name)}
    {}

    bool Symbolic::hashCode(HashCode& hashCode) {
        hashCode = _hashCode;
        return true;
    }

    std::string Symbolic::name() {
        return _name;
    }

    void Symbolic::showOn(std::ostream& ostream) const {
        ostream << _name;
    }

}

#pragma once

#include <plx/object/HashCode.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Symbolic : public Object {
    public:
        Symbolic(const std::string& name);

        // Unique functions ------------------------------------------------

        std::string name();

        // Overridden functions --------------------------------------------

        bool hashCode(HashCode& hashCode) override;
        void showOn(std::ostream& ostream) const override;

    private:
        std::string _name;
        HashCode _hashCode;

    };

}

#pragma once

#include <plx/data/InternTable.hpp>
#include <plx/object/Symbolic.hpp>

namespace PLX {

    class Symbol : public Symbolic {
    public:
        static Symbol* create(const std::string& name);

        // Unique functions ------------------------------------------------

        static void markAll();

        // Overridden functions --------------------------------------------

        TypeId typeId() const override;

    private:
        Symbol(const std::string& name);

    };

}
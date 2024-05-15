#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Array;

    class InputStream : public Object {
    public:
        InputStream(const std::string& string);

        // Unique functions ------------------------------------------------

        bool isEmpty() const;
        char next();
        Array* position() const;

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        std::string _string;
        int _line {1};
        int _col {0};
        int _pos {0};
        char _prevChar {0};
    };

}

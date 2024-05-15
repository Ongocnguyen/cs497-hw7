#pragma once

#include <string>

#include <plx/object/Object.hpp>

namespace PLX {

    class Queue;

    class String : public Object {
    public:
        String(const std::string& string);

        // Unique functions ------------------------------------------------

        std::string value();

        static HashCode stringHashCode(const std::string& s);

        Queue* split(String* delimiter);
        bool startsWith(const std::string& prefix);
        String* stripLeft(char c);
        String* substring(size_t startPos, size_t nChars=std::string::npos);

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        void displayOn(std::ostream& ostream) const override;
        bool equals(const Object* other) const override;
        bool hashCode(HashCode& hashCode) override;
        bool index(Object* indexer, Object*& retrievedValue) override;
        bool length(int& len) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

        bool plus(Object* other, Object*& value) override;
        bool times(Object* other, Object*& value) override;

    private:
        std::string _string;

    };

}

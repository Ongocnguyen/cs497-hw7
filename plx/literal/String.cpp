#include <cassert>
#include <sstream>
#include <string>
#include <string.h>  // for strncmp

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    String::String(const std::string& string)
        : _string {string}
    {}

    std::string String::value() {
        return _string;
    }

    bool String::boolValue() const {
        return _string.length() > 0;
    }

    void String::displayOn(std::ostream& ostream) const {
        ostream << _string;
    }

    bool String::equals(const Object* other) const {
        if (other->isA(TypeId::L_STRING)) {
            const String* otherString = static_cast<const String*>(other);
            return _string == otherString->_string;
        }
        return false;
    }

    bool String::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        static const std::hash<std::string> stringHasher;
        hashCode = typeHashCode ^ (stringHasher(_string) << 1);
        return true;
    }

    bool String::index(Object* indexer, Object*& retrievedValue) {
        if (!indexer->isA(TypeId::L_INTEGER)) {
            throwException("Array", "Indexer must be an integer", 
                new Array({indexer, indexer->typeSymbol()}));
        }
        Integer* indexerInt = static_cast<Integer*>(indexer);
        int n = indexerInt->value();
        if (n < 0 || n >= static_cast<int>(_string.length())) {
            throwException("String", "Index out of range", 
                new Array({indexerInt, this}));
        }
        char c {_string[n]};
        std::string str {c};
        retrievedValue = new String(str);
        return true;
    }

    bool String::length(int& len) {
        len = _string.length();
        return true;
    }

    bool String::plus(Object* other, Object*& value) {
        if (other->isA(TypeId::L_STRING)) {
            String* otherString = static_cast<String*>(other);
            std::string newString {_string + otherString->_string};
            value = new String(newString);
        }
        else {
            std::stringstream ss;
            ss << other;
            std::string newString {_string + ss.str()};
            value = new String(newString);
        }
        return true;
    }
    
    Queue* String::split(String* delimiter) {
        Queue* tokens = new Queue();
        size_t start = 0, end = 0;
        int delimLength;
        assert(delimiter->length(delimLength));
        while ((end = _string.find(delimiter->_string, start)) != std::string::npos) {
            tokens->enq(new String(_string.substr(start, end - start)));
            start = end + delimLength;
        }
        // Push the last token (or the only token if no delimiter is found)
        tokens->enq(new String(_string.substr(start)));
        return tokens;
    }

    HashCode String::stringHashCode(const std::string& s) {
        HashCode hashCode = 0;
        for (size_t n=0; n<s.size(); n++) {
            hashCode = hashRotateLeft(hashCode) ^ s[n];
        }
        return hashCode;
    }

    void String::showOn(std::ostream& ostream) const {
        ostream << '"';
        for (char c : _string) {
            switch (c) {
                case '\n': ostream << "\\n"; break;
                case '\r': ostream << "\\r"; break;
                case '\t': ostream << "\\t"; break;
                default:
                    ostream << c;
                    break;
            }
        }
        ostream << '"';
    }

    bool String::startsWith(const std::string& prefix) {
        return strncmp(_string.c_str(), prefix.c_str(), prefix.size()) == 0;
    }

    String* String::stripLeft(char c) {
        size_t n;
        for (n=0; n<_string.length(); n++) {
            if (_string[n] != c) {
                break;
            }
        }
        return new String(_string.substr(n, std::string::npos));
    }

    String* String::substring(size_t startPos, size_t nChars) {
        std::string subStr = _string.substr(startPos, nChars);
        return new String(subStr);
    }

    bool String::times(Object* other, Object*& value) {
        if (!other->isA(TypeId::L_INTEGER)) {
            return false;
        }
        Integer* otherInt = static_cast<Integer*>(other);
        std::string newString {};
        int n = otherInt->value();
        while (n-- > 0) {
            newString += _string;
        }
        value = new String(newString);
        return true;
    }
    
    TypeId String::typeId() const {
        return TypeId::L_STRING;
    }

}

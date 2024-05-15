#include <plx/data/Array.hpp>
#include <plx/literal/InputStream.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX{

    InputStream::InputStream(const std::string& string)
        : _string {string}
    {}

    bool InputStream::boolValue() const {
        return !isEmpty();
    }

    bool InputStream::isEmpty() const {
        return _pos == static_cast<int>(_string.length());
    }

    char InputStream::next() {
        if (isEmpty()) {
            return 0;
        }
        char c = _string[_pos];
        _col++;
        _pos++;
        if (_prevChar == '\n') {
            _col = 1;
            _line++;
        }
        _prevChar = c;
        return c;
    }

    Array* InputStream::position() const {
        return new Array({
            new Integer(_line),
            new Integer(_col),
            new Integer(_pos)
        });
    }

    void InputStream::showOn(std::ostream& ostream) const {
        ostream << "InputStream{\"" << _string
                << "\", line=" << _line
                << ", col=" << _col
                << ", pos=" << _pos
                << '}';
    }

    TypeId InputStream::typeId() const {
        return TypeId::D_INPUTSTREAM;
    }

}

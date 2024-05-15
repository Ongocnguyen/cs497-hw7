#include <cassert>

#include <plx/object/HashCode.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    HashCode hashRotateLeft(HashCode n) {
        static int UINT_HIGH_BIT = static_cast<int>(((unsigned int)-1) / 2 + 1);
        bool g = n >= static_cast<HashCode>(UINT_HIGH_BIT);
        return (n << 1) + g;
    }

}

#include <gtest/gtest.h>

#include <plx/gc/Memory.hpp>

namespace PLX {

    class TestObject : public Object {
    };

    TEST(Memory_Test, CreateInstance) {
        Memory memory;
    }

}

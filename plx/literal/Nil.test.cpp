#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/literal/Nil.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Nil_Test : public PlxTestFixture {};

    TEST_F(Nil_Test, InstanceType) {
        EXPECT_TRUE(GLOBALS->NilObject()->isA(TypeId::L_NIL));
        EXPECT_EQ("Nil", GLOBALS->NilObject()->typeName());
    }

    TEST_F(Nil_Test, BoolValue) {
        EXPECT_FALSE(GLOBALS->NilObject()->boolValue());
    }

    TEST_F(Nil_Test, HashCode) {
        HashCode hashCode;
        EXPECT_TRUE(GLOBALS->NilObject()->hashCode(hashCode));
        EXPECT_NE(0, static_cast<int>(hashCode));
    }

}

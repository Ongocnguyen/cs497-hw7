#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/literal/Boolean.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Boolean_Test : public PlxTestFixture {};

    TEST_F(Boolean_Test, CreateInstance) {
        Boolean* bTrue = new Boolean(true);
        Boolean* bFalse = new Boolean(false);
        EXPECT_TRUE(bTrue->isA(TypeId::L_BOOLEAN));
        EXPECT_EQ("Boolean", bTrue->typeName());
        EXPECT_TRUE(bFalse->isA(TypeId::L_BOOLEAN));
        EXPECT_EQ("Boolean", bFalse->typeName());
        EXPECT_TRUE(bTrue->boolValue());
        EXPECT_FALSE(bFalse->boolValue());
    }

    TEST_F(Boolean_Test, BoolValue) {
        Boolean* bTrue = new Boolean(true);
        Boolean* bFalse = new Boolean(false);
        EXPECT_TRUE(bTrue->boolValue());
        EXPECT_FALSE(bFalse->boolValue());
    }

    TEST_F(Boolean_Test, Equals) {
        Boolean* bTrueA = new Boolean(true);
        Boolean* bTrueB = new Boolean(true);
        Boolean* bFalse = new Boolean(false);
        EXPECT_TRUE(bTrueA->equals(bTrueB));
        EXPECT_TRUE(bTrueB->equals(bTrueA));
        EXPECT_FALSE(bTrueA->equals(bFalse));
        EXPECT_FALSE(bFalse->equals(bTrueB));
    }

    TEST_F(Boolean_Test, HashCode) {
        HashCode trueHashCode;
        EXPECT_TRUE(GLOBALS->True()->hashCode(trueHashCode));
        HashCode falseHashCode;
        EXPECT_TRUE(GLOBALS->False()->hashCode(falseHashCode));
        EXPECT_NE(trueHashCode, falseHashCode);
    }

}

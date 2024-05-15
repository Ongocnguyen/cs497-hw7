#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/literal/Integer.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Integer_Test : public PlxTestFixture {};

    TEST_F(Integer_Test, CreateInstance) {
        Integer* i100 = new Integer(100);
        EXPECT_TRUE(i100->isA(TypeId::L_INTEGER));
        EXPECT_EQ("Integer", i100->typeName());
        EXPECT_EQ(100, i100->value());
    }
    
    TEST_F(Integer_Test, Equals) {
        Integer* i100a = new Integer(100);
        Integer* i100b = new Integer(100);
        Integer* i200 = new Integer(200);
        EXPECT_TRUE(i100a->equals(i100b));
        EXPECT_TRUE(i100b->equals(i100a));
        EXPECT_FALSE(i200->equals(i100a));
        EXPECT_FALSE(i200->equals(i100b));
    }

    TEST_F(Integer_Test, HashCode) {
        Integer* integers[100];
        HashCode hashCodes[100];
        for (int n=0; n<100; n++) {
            integers[n] = new Integer(n - 50);
            EXPECT_TRUE(integers[n]->hashCode(hashCodes[n]));
        }
        for (int n=0; n<99; n++) {
            for (int m=n+1; m<100; m++) {
                EXPECT_NE(hashCodes[n], hashCodes[m]);
            }
        }
    }

    TEST_F(Integer_Test, BoolValue) {
        Integer* i0 = new Integer(0);
        Integer* i1 = new Integer(1);
        EXPECT_FALSE(i0->boolValue());
        EXPECT_TRUE(i1->boolValue());
    }

}

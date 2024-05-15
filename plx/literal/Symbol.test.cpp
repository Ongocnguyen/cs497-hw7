#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/HashTable.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Symbol_Test : public PlxTestFixture {};

    TEST_F(Symbol_Test, CreateInstance) {
        Symbol* symbol1 = Symbol::create("Abc");
        EXPECT_TRUE(symbol1->isA(TypeId::L_SYMBOL));
        EXPECT_EQ("Symbol", symbol1->typeName());
        Symbol* symbol2 = Symbol::create("Abc");
        // This asserts that both symbols are the same object
        ASSERT_EQ(static_cast<void*>(symbol1), static_cast<void*>(symbol2));
        HashCode hashCode1;
        EXPECT_TRUE(symbol1->hashCode(hashCode1));
        HashCode hashCode2;
        EXPECT_TRUE(symbol2->hashCode(hashCode2));
        EXPECT_NE(0, hashCode1);
        EXPECT_NE(0, hashCode2);
        EXPECT_EQ(hashCode1, hashCode2);
        Symbol* symbol3 = Symbol::create("Def");
        HashCode hashCode3;
        EXPECT_TRUE(symbol3->hashCode(hashCode3));
        EXPECT_NE(hashCode1, hashCode3);
        EXPECT_NE(hashCode2, hashCode3);
    }

    TEST_F(Symbol_Test, ShowOn) {
        {
            std::stringstream ss;
            Symbol* sym1 = Symbol::create("Abc");
            ss << sym1;
            EXPECT_EQ("Abc", ss.str());
        }
        {
            std::stringstream ss;
            Symbol* sym1 = Symbol::create("Def");
            ss << sym1;
            EXPECT_EQ("Def", ss.str());
        }
    }

}

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/literal/P_Literal.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    class P_Literal_Test : public PlxTestFixture {};

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);

    TEST_F(P_Literal_Test, pBoolean_true) {
        Array* token1 = buildToken("Boolean", GLOBALS->True(), 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pBoolean(tokens, value));
        EXPECT_EQ(*GLOBALS->True(), *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Literal_Test, pBoolean_false) {
        Array* token1 = buildToken("Boolean", GLOBALS->False(), 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pBoolean(tokens, value));
        EXPECT_EQ(*GLOBALS->False(), *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Literal_Test, pIntger) {
        Integer* i100 = new Integer(100);
        Array* token1 = buildToken("Integer", i100, 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pInteger(tokens, value));
        EXPECT_EQ(*i100, *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Literal_Test, pNil) {
        Array* token1 = buildToken("Nil", GLOBALS->NilObject(), 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pNil(tokens, value));
        EXPECT_EQ(*GLOBALS->NilObject(), *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Literal_Test, pString) {
        String* abc = new String("abc");
        Array* token1 = buildToken("String", abc, 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pString(tokens, value));
        EXPECT_EQ(*abc, *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Literal_Test, pSymbol) {
        Symbol* abc = Symbol::create("Abc");
        Array* token1 = buildToken("Symbol", abc, 1, 0, 0);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pSymbol(tokens, value));
        EXPECT_EQ(*abc, *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

}

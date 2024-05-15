#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/data/P_Data.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Binding_Test : public PlxTestFixture {};

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);

    TEST_F(P_Binding_Test, Success) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("nil = nil", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pBinding(tokens, value));
        Object* expectedValue = new Triple(GLOBALS->NilObject(), GLOBALS->NilObject());
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Binding_Test, Failure1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("nil=", tokens, errorValue));
        Object* value;
        ASSERT_THROW(pBinding(tokens, value), Array*);
    }

    TEST_F(P_Binding_Test, BindingList) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("x=100, y=200", tokens, errorValue));
        Object* value;
        EXPECT_TRUE(pBindingList(0, tokens, value));
        Object* expectedValue = new Triple(
            Identifier::create("x"),
            new Integer(100),
            new Triple(
                Identifier::create("y"),
                new Integer(200)));
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Binding_Test, BindingList_MinimumNotSatisfied) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("x=100", tokens, errorValue));
        Object* value;
        EXPECT_FALSE(pBindingList(2, tokens, value));
    }

    TEST_F(P_Binding_Test, BindingList_Failure) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("x=100,", tokens, errorValue));
        Object* value;
        EXPECT_THROW(pBindingList(0, tokens, value), Array*);
    }

    TEST_F(P_Binding_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "x=",
            "x=100,",
        };
        auto parserLambda = [](List*& tokens, Object*& value) { return pBindingList(0, tokens, value); };
        testErrorStrings("pBindingList", parserLambda, errorStrings);
    }

}

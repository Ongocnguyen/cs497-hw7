#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Identifier_Test : public PlxTestFixture {};

    TEST_F(P_Identifier_Test, Identifier) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("x", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pIdentifier(tokens, value));
        Object* expectedValue = Identifier::create("x");
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Identifier_Test, Fail1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("123", tokens, errorValue));
        Object* value;
        ASSERT_FALSE(pIdentifier(tokens, value));
    }

}

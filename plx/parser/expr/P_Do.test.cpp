#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Do_Test : public PlxTestFixture {};

    TEST_F(P_Do_Test, Empty) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string doString {"do end"};
        ASSERT_TRUE(lexer->tokenize(doString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pDo(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(doString, ss.str());
    }

    TEST_F(P_Do_Test, Expr1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string doString {"do 123 end"};
        ASSERT_TRUE(lexer->tokenize(doString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pDo(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(doString, ss.str());
    }

   TEST_F(P_Do_Test, Expr2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string doString {"do 123 456 end"};
        ASSERT_TRUE(lexer->tokenize(doString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pDo(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(doString, ss.str());
    }

    TEST_F(P_Do_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "do",
            "do 123",
            "do 123 456"
        };
        testErrorStrings("pDo", pDo, errorStrings);
    }

}

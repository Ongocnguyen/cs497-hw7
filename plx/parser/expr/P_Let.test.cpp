#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Let_Test : public PlxTestFixture {};

    TEST_F(P_Let_Test, Let_Bindings1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"let x=100"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pLet(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(letString, ss.str());
    }

    TEST_F(P_Let_Test, Let_Bindings2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"let x=100, y=200"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pLet(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(letString, ss.str());
    }

    TEST_F(P_Let_Test, LetIn_Bindings1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"let x=100 in x"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pLet(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(letString, ss.str());
    }

    TEST_F(P_Let_Test, LetIn_Bindings2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"let x=100, y=200 in {x, y}"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pLet(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(letString, ss.str());
    }

    TEST_F(P_Let_Test, FailStrings_Let) {
        std::initializer_list<std::string> errorStrings {
            "let",
            "let x"
        };
        testErrorStrings("pLet", pLet, errorStrings);
    }

    TEST_F(P_Let_Test, FailStrings_LetIn) {
        std::initializer_list<std::string> errorStrings {
            "let",
            "let x",
            // "let x=100",
            "let x=100 in"
        };
        testErrorStrings("pLet", pLet, errorStrings);
    }

}

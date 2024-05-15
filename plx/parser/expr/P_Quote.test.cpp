#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/lexer/Lexer.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Quote_Test : public PlxTestFixture {};

    TEST_F(P_Quote_Test, Quote) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"'x'"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pQuote(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(letString, ss.str());
    }

    TEST_F(P_Quote_Test, Quote_MissingQuote) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string letString {"'x"};
        ASSERT_TRUE(lexer->tokenize(letString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pQuote(tokens, value), Array*);
    }

    TEST_F(P_Quote_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "'",
            "'abc"
        };
        testErrorStrings("pQuote", pQuote, errorStrings);
    }

}

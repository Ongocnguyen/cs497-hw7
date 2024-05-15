#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_BinOpExpr_Test : public PlxTestFixture {};

    bool pBinOpExpr(List*& tokens, Object*& value);

    TEST_F(P_BinOpExpr_Test, Success) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string binOpString = "123 + 456";
        ASSERT_TRUE(lexer->tokenize(binOpString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pBinOpExpr(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(binOpString, ss.str());
    }

    TEST_F(P_BinOpExpr_Test, Fail1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string binOpString = "123 +";
        ASSERT_TRUE(lexer->tokenize(binOpString, tokens, errorValue));
        Object* value;
        EXPECT_THROW(pBinOpExpr(tokens, value), Array*);
    }

    TEST_F(P_BinOpExpr_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "123 +"
        };
        testErrorStrings("pBinOpExpr", pBinOpExpr, errorStrings);
    }

}

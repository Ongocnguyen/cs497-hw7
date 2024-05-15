#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_TryCatch_Test : public PlxTestFixture {};

    bool pTryCatch(List*& tokens, Object*& value);

    TEST_F(P_TryCatch_Test, Rule1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string matchString = "try 10 catch 10=100 end";
        ASSERT_TRUE(lexer->tokenize(matchString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pTryCatch(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(matchString, ss.str());
    }

    TEST_F(P_TryCatch_Test, Rule2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string matchString = "try 10 catch 10=100 | 20=200 end";
        ASSERT_TRUE(lexer->tokenize(matchString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pTryCatch(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(matchString, ss.str());
    }

    TEST_F(P_TryCatch_Test, Rule2Finally) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string matchString = "try 10 catch 10=100 | 20=200 finally 300 end";
        ASSERT_TRUE(lexer->tokenize(matchString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pTryCatch(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(matchString, ss.str());
    }

    TEST_F(P_TryCatch_Test, FailStrings_LetIn) {
        std::initializer_list<std::string> errorStrings {
           "try",
            "try 10",
            "try 10 end",
            "try 10 catch",
            "try 10 catch end",
            "try 10 catch 10=100 |",
            "try 10 catch 10=100 | 20=",
            "try 10 catch 10=100 | 20=200",
            "try 10 catch 10=100 | 20=200 finally",
            "try 10 catch 10=100 | 20=200 finally 30",
            "try 10 catch 10=100 | 20=200 finally end"
        };
        testErrorStrings("pTryCatch", pTryCatch, errorStrings);
    }
}

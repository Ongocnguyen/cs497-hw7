#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Match_Test : public PlxTestFixture {};

    bool pMatch(List*& tokens, Object*& value);

    TEST_F(P_Match_Test, Rule1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string matchString = "match x in 10=100 end";
        ASSERT_TRUE(lexer->tokenize(matchString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pMatch(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(matchString, ss.str());
    }

    TEST_F(P_Match_Test, Rule2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string matchString = "match x in 10=100 | 20=200 end";
        ASSERT_TRUE(lexer->tokenize(matchString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pMatch(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(matchString, ss.str());
    }

    TEST_F(P_Match_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "match",
            "match x",
            "match x in",
            "match x in 10=100 |",
            "match x in 10=100 | 20=",
            "match x in 10=100 | 20=200"
        };
        testErrorStrings("pMatch", pMatch, errorStrings);
    }

}

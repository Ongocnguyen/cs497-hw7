#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/lexer/Lexer.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Throw_Test : public PlxTestFixture {};

    bool pThrow(List*& tokens, Object*& value);

    TEST_F(P_Throw_Test, ThrowNil) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string throwString = "throw nil";
        ASSERT_TRUE(lexer->tokenize(throwString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pThrow(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(throwString, ss.str());
    }

    TEST_F(P_Throw_Test, Fail1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string throwString = "throw";
        ASSERT_TRUE(lexer->tokenize(throwString, tokens, errorValue));
        Object* value;
        EXPECT_THROW(pThrow(tokens, value), Array*);
    }

    TEST_F(P_Throw_Test, FailStrings_LetIn) {
        std::initializer_list<std::string> errorStrings {
            "throw"
        };
        testErrorStrings("pThrow", pThrow, errorStrings);
    }

}

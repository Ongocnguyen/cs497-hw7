#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_If_Test : public PlxTestFixture {};

    TEST_F(P_If_Test, If) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string ifString {"if true then 100 else 200"};
        ASSERT_TRUE(lexer->tokenize(ifString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pIf(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(ifString, ss.str());
    }

    TEST_F(P_If_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "if",
            "if true",
            "if then",
            "if then else"
            "if true then",
            "if true then 100",
            "if true then 100 else"
        };
        testErrorStrings("pIf", pIf, errorStrings);
    }

}

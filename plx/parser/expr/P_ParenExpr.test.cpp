#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_ParenExpr_Test : public PlxTestFixture {};

    TEST_F(P_ParenExpr_Test, ParenExpr) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string parenExprString {"(x)"};
        ASSERT_TRUE(lexer->tokenize(parenExprString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pParenExpr(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(parenExprString, ss.str());
    }

    TEST_F(P_ParenExpr_Test, ParenExpr_Failure1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string parenExprString {"(x"};
        ASSERT_TRUE(lexer->tokenize(parenExprString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pParenExpr(tokens, value), Array*);
    }

    TEST_F(P_ParenExpr_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "("
        };
        testErrorStrings("pParenExpr", pParenExpr, errorStrings);
    }

}

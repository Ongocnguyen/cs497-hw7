#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Apply_Test : public PlxTestFixture {};

    TEST_F(P_Apply_Test, Identifier_EmptyArgumentList) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"foo()"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, Identifier_ArgumentList1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"foo(123)"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, Identifier_ArgumentList2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"foo(123, 456)"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, ParenExpr_EmptyArgumentList) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"(foo)()"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, ParenExpr_ArgumentList1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"(foo)(123)"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, ParenExpr_ArgumentList2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string applyString {"(foo)(123, 456)"};
        ASSERT_TRUE(lexer->tokenize(applyString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pApply(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_APPLY));
    }

    TEST_F(P_Apply_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "foo(",
            "(foo)(",
            "foo(123",
            "(foo)(123"
        };
        testErrorStrings("pApply", pApply, errorStrings);
    }

}

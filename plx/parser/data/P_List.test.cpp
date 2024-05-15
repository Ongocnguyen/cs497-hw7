#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/data/P_Data.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_List_Test : public PlxTestFixture {};

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);

    TEST_F(P_List_Test, Length0) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("[]", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pList(tokens, value));
        List* expectedList = GLOBALS->EmptyList();
        EXPECT_EQ(*expectedList, *value);
    }

    TEST_F(P_List_Test, Length1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("[nil]", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pList(tokens, value));
        List* expectedList = List::create({GLOBALS->NilObject()});
        EXPECT_EQ(*expectedList, *value);
    }

    TEST_F(P_List_Test, Length2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("[nil, nil]", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pList(tokens, value));
        List* expectedList = List::create({GLOBALS->NilObject(), GLOBALS->NilObject()});
        EXPECT_EQ(*expectedList, *value);
    }

    TEST_F(P_List_Test, Length3) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("[nil, nil, nil]", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pList(tokens, value));
        List* expectedList = List::create({GLOBALS->NilObject(), GLOBALS->NilObject(), GLOBALS->NilObject()});
        EXPECT_EQ(*expectedList, *value);
    }

    TEST_F(P_List_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "[",
            "[,]"
            "[nil,",
            "[nil, nil",
        };
        testErrorStrings("pList", pList, errorStrings);
    }

}

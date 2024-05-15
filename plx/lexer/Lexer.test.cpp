#include <iostream>

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>

namespace PLX {

    class Lexer_Test : public PlxTestFixture {};

    Array* positionArray(int line, int col, int pos);
    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);
    Array* buildEoiToken(Array* position);

    Array* buildEoiToken(int line, int col, int pos) {
        Array* position = new Array({
            new Integer(line),
            new Integer(col),
            new Integer(pos)
        });
        return buildEoiToken(position);
    }

    TEST_F(Lexer_Test, CreateInstance) {
        Lexer* lexer1 = new Lexer();
        EXPECT_TRUE(lexer1->isA(TypeId::T_LEXER));
        EXPECT_EQ("Lexer", lexer1->typeName());
    }

    TEST_F(Lexer_Test, Boolean_True) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("true", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Boolean", GLOBALS->True(), 1, 1, 1),
            buildEoiToken(1, 4, 4)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Boolean_False) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("false", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Boolean", GLOBALS->False(), 1, 1, 1),
            buildEoiToken(1, 5, 5)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Nil) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("nil", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Nil", GLOBALS->NilObject(), 1, 1, 1),
            buildEoiToken(1, 3, 3)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Identifier) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("abc", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Identifier", Identifier::create("abc"), 1, 1, 1),
            buildEoiToken(1, 3, 3)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Symbol) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("Abc", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Symbol", Symbol::create("Abc"), 1, 1, 1),
            buildEoiToken(1, 3, 3)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Integer) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("123", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Integer", new Integer(123), 1, 1, 1),
            buildEoiToken(1, 3, 3)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Integer_Positive) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("+123", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Integer", new Integer(123), 1, 1, 1),
            buildEoiToken(1, 4, 4)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Integer_Negative) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("-123", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Integer", new Integer(-123), 1, 1, 1),
            buildEoiToken(1, 4, 4)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, String_Empty) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("\"\"", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("String", new String(""), 1, 1, 1),
            buildEoiToken(1, 2, 2)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, String_Nonempty) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("\"abc\"", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("String", new String("abc"), 1, 1, 1),
            buildEoiToken(1, 5, 5)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, String_EscapedChars) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("\"\\n\\r\\t\\\"\\a\\b\\c\"", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("String", new String("\n\r\t\"abc"), 1, 1, 1),
            buildEoiToken(1, 16, 16)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, String_UnterminatedString) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_FALSE(lexer->tokenize("\"abc", tokens, errorValue));
        Array* expectedArray = new Array({
            new String("Unterminated string"),
            positionArray(1, 1, 1)
        });
        ASSERT_EQ(*expectedArray, *errorValue);
    }

    TEST_F(Lexer_Test, Operator) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("<-=+=->", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Identifier", Identifier::create("<-=+=->"), 1, 1, 1),
            buildEoiToken(1, 7, 7)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Reserved) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("end", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Reserved", new String("end"), 1, 1, 1),
            buildEoiToken(1, 3, 3)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, MultipleTokens1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("abc 123", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Identifier", Identifier::create("abc"), 1, 1, 1),
            buildToken("Integer", new Integer(123), 1, 5, 5),
            buildEoiToken(1, 7, 7)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, MultipleTokens2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("if true then 100 else 200", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Reserved", new String("if"), 1, 1, 1),
            buildToken("Boolean", GLOBALS->True(), 1, 4, 4),
            buildToken("Reserved", new String("then"), 1, 9, 9),
            buildToken("Integer", new Integer(100), 1, 14, 14),
            buildToken("Reserved", new String("else"), 1, 18, 18),
            buildToken("Integer", new Integer(200), 1, 23, 23),
            buildEoiToken(1, 25, 25)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

    TEST_F(Lexer_Test, Newlines) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("abc\ndef\nghi", tokens, errorValue));
        List* expectedTokens = List::create({
            buildToken("Identifier", Identifier::create("abc"), 1, 1, 1),
            buildToken("Identifier", Identifier::create("def"), 2, 1, 5),
            buildToken("Identifier", Identifier::create("ghi"), 3, 1, 9),
            buildEoiToken(3, 3, 11)
        });
        ASSERT_EQ(*expectedTokens, *tokens);
    }

}

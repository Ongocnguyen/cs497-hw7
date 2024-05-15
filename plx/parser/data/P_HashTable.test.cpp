#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/data/P_Data.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_HashTable_Test : public PlxTestFixture {};

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);

    TEST_F(P_HashTable_Test, Count0) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("#{}", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pHashTable(tokens, value));
        EXPECT_TRUE(isEoi(tokens));
        HashTable* expectedHashTable = new HashTable();
        EXPECT_EQ(*expectedHashTable, *value);
    }

    TEST_F(P_HashTable_Test, Count1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("#{x=100}", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pHashTable(tokens, value));
        EXPECT_TRUE(isEoi(tokens));
        HashTable* expectedHashTable = new HashTable();
        expectedHashTable->put(Identifier::create("x"), new Integer(100));
        EXPECT_EQ(*expectedHashTable, *value);
    }

    TEST_F(P_HashTable_Test, Count2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("#{a=100, b=200}", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pHashTable(tokens, value));
        EXPECT_TRUE(isEoi(tokens));
        HashTable* expectedHashTable = new HashTable();
        expectedHashTable->put(Identifier::create("a"), new Integer(100));
        expectedHashTable->put(Identifier::create("b"), new Integer(200));
        EXPECT_EQ(*expectedHashTable, *value);
    }

    TEST_F(P_HashTable_Test, Count3) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        ASSERT_TRUE(lexer->tokenize("#{a=100, b=200, c=300}", tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pHashTable(tokens, value));
        EXPECT_TRUE(isEoi(tokens));
        HashTable* expectedHashTable = new HashTable();
        expectedHashTable->put(Identifier::create("a"), new Integer(100));
        expectedHashTable->put(Identifier::create("b"), new Integer(200));
        expectedHashTable->put(Identifier::create("c"), new Integer(300));
        EXPECT_EQ(*expectedHashTable, *value);
    }

    TEST_F(P_HashTable_Test, EmptyTokenList) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_FALSE(pHashTable(tokens, value));
    }

    TEST_F(P_HashTable_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "#{",
            "#{a",
            "#{a=",
            "#{a=100",
            "#{a=100,",
        };
        testErrorStrings("pHashTable", pHashTable, errorStrings);
    }

}

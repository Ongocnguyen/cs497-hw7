#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    class P_Primitive_Test : public PlxTestFixture {};

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);
    Array* buildEoiToken(int line, int col, int pos);

    bool testParserPass(List*& tokens, Object*& value) {
        tokens = tokens->restAsList();
        value = GLOBALS->NilObject();
        return true;
    }

    bool testParserPassUntilEoi(List*& tokens, Object*& value) {
        if (isEoi(tokens)) {
            return false;
        }
        tokens = tokens->restAsList();
        value = GLOBALS->NilObject();
        return true;
    }

    bool testParserFail(List*& tokens, Object*& value) {
        (void)tokens;
        value = GLOBALS->NilObject();
        return false;
    }

    // primitive parsers ---------------------------------------------------

    TEST_F(P_Primitive_Test, pSpot_ParseSuccess) {
        Array* token1 = buildToken("Abc", new Integer(123), 4, 5, 6);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pSpot(Symbol::create("Abc"), tokens, value));
        EXPECT_EQ(*token1, *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Primitive_Test, pSpot_ParseFailure) {
        Array* token1 = buildToken("Abc", new Integer(123), 4, 5, 6);
        List* tokens = List::create({token1});
        List* savedTokens = tokens;
        Object* value;
        ASSERT_FALSE(pSpot(Symbol::create("Def"), tokens, value));
        EXPECT_EQ(savedTokens, tokens);
    }

    TEST_F(P_Primitive_Test, pSpot_EmptyTokenList) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_FALSE(pSpot(Symbol::create("Abc"), tokens, value));
    }

    TEST_F(P_Primitive_Test, pSpotValue_ParseSucces) {
        Integer* i123 = new Integer(123);
        Array* token1 = buildToken("Abc", i123, 4, 5, 6);
        List* tokens = List::create({token1});
        Object* value;
        ASSERT_TRUE(pSpotValue(Symbol::create("Abc"), new Integer(123), tokens, value));
        EXPECT_EQ(*GLOBALS->ParseIgnoreSymbol(), *value);
        EXPECT_TRUE(tokens->isEmpty());
    }

    TEST_F(P_Primitive_Test, pSpotValue_ParseFailure_DifferentType) {
        Array* token1 = buildToken("Abc", new Integer(123), 4, 5, 6);
        List* tokens = List::create({token1});
        List* savedTokenList = tokens;
        Object* value;
        ASSERT_FALSE(pSpotValue(Symbol::create("Def"), new Integer(123), tokens, value));
        EXPECT_EQ(savedTokenList, tokens);
    }

    TEST_F(P_Primitive_Test, pSpotValue_ParseFailure_SameTypeDifferentValue) {
        Array* token1 = buildToken("Abc", new Integer(123), 4, 5, 6);
        List* tokens = List::create({token1});
        List* savedTokenList = tokens;
        Object* value;
        ASSERT_FALSE(pSpotValue(Symbol::create("Abc"), new Integer(456), tokens, value));
        EXPECT_EQ(savedTokenList, tokens);
    }

    TEST_F(P_Primitive_Test, pSpotValue_EmptyTokenList) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_FALSE(pSpotValue(Symbol::create("Abc"), GLOBALS->NilObject(), tokens, value));
    }

    TEST_F(P_Primitive_Test, pMany_Empty) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_TRUE(pMany(testParserFail, 0, tokens, value));
        EXPECT_TRUE(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        EXPECT_EQ(0, valueQueue->length());
    }

    TEST_F(P_Primitive_Test, pMany_Success1) {
        List* tokens = List::create({
            buildToken("Abc", new Integer(123), 1, 0, 0),
            buildEoiToken(1, 3, 3)
        });
        Object* value;
        ASSERT_TRUE(pMany(testParserPassUntilEoi, 0, tokens, value));
        EXPECT_TRUE(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        EXPECT_EQ(1, valueQueue->length());
    }


    TEST_F(P_Primitive_Test, pMany_Success2) {
        List* tokens = List::create({
            buildToken("Abc", new Integer(123), 1, 0, 0),
            buildToken("Def", new Integer(456), 1, 4, 4),
            buildEoiToken(1, 7, 7)
        });
        Object* value;
        ASSERT_TRUE(pMany(testParserPassUntilEoi, 0, tokens, value));
        EXPECT_TRUE(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        EXPECT_EQ(2, valueQueue->length());
    }

    TEST_F(P_Primitive_Test, pOneOf_Empty) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {};
        ASSERT_FALSE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pOneOf_Pass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass};
        ASSERT_TRUE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pOneOf_PassPass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass, testParserPass};
        ASSERT_TRUE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pOneOf_FailPass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserFail, testParserPass};
        ASSERT_TRUE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pOneOf_PassFail) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass, testParserFail};
        ASSERT_TRUE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pOneOf_FailFail) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserFail, testParserFail};
        ASSERT_FALSE(pOneOf(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pSepBy_Empty) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_TRUE(pSepBy(testParserFail, testParserPass, 0, tokens, value));
        EXPECT_TRUE(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        EXPECT_EQ(0, valueQueue->length());
    }

    TEST_F(P_Primitive_Test, pSepBy_PassFail_MinCount) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_TRUE(pSepBy(testParserPass, testParserFail, 0, tokens, value));
        EXPECT_TRUE(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        EXPECT_EQ(1, valueQueue->length());
    }

    TEST_F(P_Primitive_Test, pSepBy_PassFail_NoMinCount) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        ASSERT_FALSE(pSepBy(testParserPass, testParserFail, 2, tokens, value));
    }

    static bool _pInt(List*& tokens, Object*& value) {
        return pSpotAndStrip(Symbol::create("Integer"), tokens, value);
    }

    static bool _pComma(List*& tokens, Object*& value) {
        return pSpotValue(Symbol::create("Special"), ",", tokens, value);
    }

    TEST_F(P_Primitive_Test, pSepBy_1) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* tokens = List::create({
            buildToken("Integer", i100, 1, 0, 0),
            buildToken("Special", new String(","), 1, 4, 4),
            buildToken("Integer", i200, 1, 6, 6)
        });
        Object* value;
        ASSERT_TRUE(pSepBy(_pInt, _pComma, 2, tokens, value));
        Queue* expectedQueue = new Queue({i100, i200});
        EXPECT_EQ(*expectedQueue, *value);
    }

    TEST_F(P_Primitive_Test, pSepBy_Fail1) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* tokens = List::create({
            buildToken("Integer", i100, 1, 0, 0),
            buildToken("Special", new String(","), 1, 4, 4),
            buildToken("Integer", i200, 1, 0, 0),
            buildEoiToken(1, 5, 5)
        });
        List* savedTokens = tokens;
        Object* value;
        ASSERT_FALSE(pSepBy(_pInt, _pComma, 3, tokens, value));
        EXPECT_EQ(savedTokens, tokens);
    }

    TEST_F(P_Primitive_Test, pSepBy_Fail2) {
        Integer* i100 = new Integer(100);
        List* tokens = List::create({
            buildToken("Integer", i100, 1, 0, 0),
            buildToken("Special", new String(","), 1, 4, 4),
            buildEoiToken(1, 5, 5)
        });
        Object* value;
        ASSERT_THROW(pSepBy(_pInt, _pComma, 0, tokens, value), Array*);
    }

    TEST_F(P_Primitive_Test, pSeq_Empty) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {};
        ASSERT_TRUE(pSeq(parsers, tokens, value));
        Object* expectedValue = new Queue();
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Primitive_Test, pSeq_Pass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass};
        ASSERT_TRUE(pSeq(parsers, tokens, value));
        Object* expectedValue = GLOBALS->NilObject();
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Primitive_Test, pSeq_PassPass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass, testParserPass};
        ASSERT_TRUE(pSeq(parsers, tokens, value));
        Object* expectedValue = new Queue({GLOBALS->NilObject(), GLOBALS->NilObject()});
        EXPECT_EQ(*expectedValue, *value);
    }

    TEST_F(P_Primitive_Test, pSeq_FailPass) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserFail, testParserPass};
        ASSERT_FALSE(pSeq(parsers, tokens, value));
    }

    TEST_F(P_Primitive_Test, pSeq_PassFail) {
        List* tokens = GLOBALS->EmptyList();
        Object* value;
        std::initializer_list<Parser> parsers {testParserPass, testParserFail};
        ASSERT_FALSE(pSeq(parsers, tokens, value));
    }

}

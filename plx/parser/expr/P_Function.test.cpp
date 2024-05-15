#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

    class P_Function_Test : public PlxTestFixture {};

    TEST_F(P_Function_Test, Anonymous_NoParams) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun () = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Anonymous_OneParam) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (x) = x"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Named_NoParams) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo() = x"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_NAMEDFUNCTION));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Named_OneParam) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo(x) = x"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        EXPECT_TRUE(value->isA(TypeId::E_NAMEDFUNCTION));
        // It doesn't make sense to compare two Expressions for equality,
        // so compare their string representations instead.
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Named_Failure1) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo(x) ="};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Named_Failure2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo(x)"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Named_Failure3) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo(x"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Named_Failure4) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo("};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Named_Failure5) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun foo"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Named_Failure6) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_THROW(pFunction(tokens, value), Array*);
    }

    TEST_F(P_Function_Test, Rules2) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun () = nil | (x) = x"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Rules3) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun () = nil | (x) = x | (y) = y"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Empty) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun () = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Identifier) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (x) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Integer) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (123) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Nil) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (nil) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Boolean) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (true) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_String) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (\"abc\") = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Symbol) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (Abc) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_Array) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun ({a, b}) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, ParameterType_List) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun ([a, b]) = nil"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, Multiple) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        std::string funString {"fun (a, {b, c}, [d, e]) = nil | ({1, x}) = true"};
        ASSERT_TRUE(lexer->tokenize(funString, tokens, errorValue));
        Object* value;
        ASSERT_TRUE(pFunction(tokens, value));
        std::stringstream ss;
        ss << value;
        EXPECT_EQ(funString, ss.str());
    }

    TEST_F(P_Function_Test, FailStrings) {
        std::initializer_list<std::string> errorStrings {
            "fun",
            "fun (",
            "fun )",
            "fun (a",
            "fun ()",
            "fun () =",
            "fun (a, b",
            "fun (a, b)"
        };
        testErrorStrings("pFunction", pFunction, errorStrings);
    }

}
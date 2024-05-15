#include <sstream>

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/BinOpExpr.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/If.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    class P_Any_Test : public PlxTestFixture {};

    TEST_F(P_Any_Test, Objects) {
        // This ensures that P_Any can distinguish different object types.
        static const std::initializer_list<std::tuple<std::string, TypeId>> pairs {
            {"nil", TypeId::L_NIL},
            {"true", TypeId::L_BOOLEAN},
            {"false", TypeId::L_BOOLEAN},
            {"123", TypeId::L_INTEGER},
            {"\"abc\"", TypeId::L_STRING},
            {"Abc", TypeId::L_SYMBOL},
            {"{}", TypeId::D_ARRAY},
            {"[]", TypeId::D_LIST},
            {"#{}", TypeId::D_HASHTABLE},
            {"~[]", TypeId::D_QUEUE},
            {"A = 100", TypeId::E_BINOPEXPR},
            {"abc", TypeId::E_IDENTIFIER},
            {"f[x]", TypeId::E_INDEXOP},
            {"f[x][y]", TypeId::E_INDEXOP},
            {"f[x][y][z]", TypeId::E_INDEXOP},
            {"f(x)", TypeId::E_APPLY},
            {"f(x)(y)", TypeId::E_APPLY},
            {"f(x)(y)(z)", TypeId::E_APPLY}
            // more Expressions are tested in P_Expr
        };
        Lexer* lexer = new Lexer();
        for (const std::tuple<std::string, TypeId>& pair : pairs) {
            std::string inputString = std::get<0>(pair);
            TypeId typeId = std::get<1>(pair);
            List* tokens;
            Object* errorValue;
            ASSERT_TRUE(lexer->tokenize(inputString, tokens, errorValue));
            Object* value;
            List* savedTokens = tokens;
            ASSERT_TRUE(pAny(tokens, value));
            ASSERT_TRUE(isEoi(tokens))
                << " | Input string  = \"" << inputString << "\"\n"
                << " | Extra tokens  = " << tokens << "\n"
                << " | Tokens        = " << savedTokens << "\n";
            EXPECT_TRUE(value->isA(typeId))
                << " | Input string  = \"" << inputString << "\"\n"
                << " | Expected type = " << TYPE_NAMES[typeId] << "\n"
                << " | Actual type   = " << value->typeName() << "\n";
            std::stringstream ss;
            ss << value;
            EXPECT_EQ(inputString, ss.str());
        }
    }

    TEST_F(P_Any_Test, IndexesAndFuntionCalls) {
        std::initializer_list<std::string> inputStrings {
            "f(x)[y]",
            "f(x)[y](z)",
            "f[x](y)",
            "f[x](y)[z]",
        };
        Lexer* lexer = new Lexer();
        for (std::string inputString : inputStrings) {
            List* tokens;
            Object* errorValue;
            ASSERT_TRUE(lexer->tokenize(inputString, tokens, errorValue));
            Object* value;
            ASSERT_TRUE(pAny(tokens, value));
            ASSERT_TRUE(isEoi(tokens));
            std::stringstream ss;
            ss << value;
            EXPECT_EQ(inputString, ss.str());
        }
    }

}

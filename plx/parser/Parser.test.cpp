#include <gtest/gtest.h>

#include <plx/lexer/Lexer.hpp>
#include <plx/data/List.hpp>
#include <plx/parser/Parser.test.hpp>

namespace PLX {

        void testErrorStrings(const std::string& name, Parser parser, std::initializer_list<std::string>& strings) {
        Lexer* lexer = new Lexer();
        List* tokens;
        Object* errorValue;
        for (const std::string& failString : strings) {
            ASSERT_TRUE(lexer->tokenize(failString, tokens, errorValue)) << "Parser '" << name << "' failure while tokenizing string '" << failString << "'";
            Object* value;
            ASSERT_THROW(parser(tokens, value), Array*) << "Parser '" << name << "' did not fail on string '" << failString << "'";
        }
    }

}

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/HashTable.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/repl/ReadEvalPrint.hpp>

namespace PLX {

    class ReadEvalPrint_Test : public PlxTestFixture {};

    TEST_F(ReadEvalPrint_Test, CreateInstance) {
        HashTable* argMap = new HashTable();
        ReadEvalPrint* rep = new ReadEvalPrint(argMap);
        EXPECT_TRUE(rep->isA(TypeId::T_READEVALPRINT));
        EXPECT_EQ("ReadEvalPrint", rep->typeName());
    }

    // TEST_F(ReadEvalPrint_Test, Read1) {
    //     ReadEvalPrint* rep = new ReadEvalPrint();
    //     std::string inputString = "nil";
    //     Object* value;
    //     EXPECT_TRUE(rep->read(inputString, value));
    //     EXPECT_EQ(*GLOBALS->NilObject(), *value);
    // }

    // TEST_F(ReadEvalPrint_Test, LexerError) {
    //     ReadEvalPrint* rep = new ReadEvalPrint();
    //     std::string inputString = "\"abc";
    //     Object* value;
    //     EXPECT_FALSE(rep->read(inputString, value));
    // }

    // TEST_F(ReadEvalPrint_Test, ParserError) {
    //     ReadEvalPrint* rep = new ReadEvalPrint();
    //     std::string inputString = "}";
    //     Object* value;
    //     EXPECT_FALSE(rep->read(inputString, value));
    // }

}

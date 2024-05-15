#include <gtest/gtest.h>

#include <plx/object/Globals.hpp>

namespace PLX {

    class Globals_Test : public ::testing::Test {
    };

    TEST_F(Globals_Test, CreateInstance) {
        Globals* savedGlobals = GLOBALS;
        GLOBALS = new Globals();
        GLOBALS->initializeAll();
        EXPECT_NE(nullptr, GLOBALS->Gc());
        EXPECT_NE(nullptr, GLOBALS->NilObject());
        delete GLOBALS;
        GLOBALS = savedGlobals;
    }

}

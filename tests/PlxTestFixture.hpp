#pragma once

#include <iostream>

#include <gtest/gtest.h>

#include <plx/object/Globals.hpp>

namespace PLX {

    class PlxTestFixture : public ::testing::Test {
    public:

        static void SetUpTestSuite() {
            _savedGlobals = GLOBALS;
            GLOBALS = new Globals();
            GLOBALS->initializeAll();
        }

        void SetUp() override {
        }

        void TearDown() override {
        }

        static void TearDownTestSuite() {
            delete GLOBALS;
            GLOBALS = _savedGlobals;
        }

    private:
        static Globals* _savedGlobals;

    };

}

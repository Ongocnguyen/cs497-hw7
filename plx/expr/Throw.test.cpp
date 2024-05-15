#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/expr/Throw.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Throw_Test : public PlxTestFixture {};

    TEST_F(Throw_Test, CreateInstance) {
        Throw* throwExpr = new Throw(GLOBALS->NilObject());
        EXPECT_TRUE(throwExpr->isA(TypeId::E_THROW));
        EXPECT_EQ("Throw", throwExpr->typeName());
    }

    TEST_F(Throw_Test, Eval) {
        VM* vm = new VM();
        Throw* throwExpr = new Throw(GLOBALS->NilObject());
        EXPECT_THROW(vm->evalExpr(throwExpr), Object*);
    }

    TEST_F(Throw_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Throw* throwExpr = new Throw(i100);
        EXPECT_FALSE(throwExpr->isMarked());
        EXPECT_FALSE(i100->isMarked());
        std::vector<Object*> objs{throwExpr};
        GC::mark(objs);
        EXPECT_FALSE(throwExpr->isMarked());
        EXPECT_TRUE(i100->isMarked());
    }

    TEST_F(Throw_Test, ShowOn) {
        Integer* i100 = new Integer(100);
        Throw* throwExpr = new Throw(i100);
        std::stringstream ss;
        ss << throwExpr;
        EXPECT_EQ("throw 100", ss.str());
    }

}

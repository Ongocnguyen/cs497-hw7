#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/vm/VM.hpp>
#include <plx/expr/Drop.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Drop_Test : public PlxTestFixture {};

    TEST_F(Drop_Test, CreateInstance) {
        Drop* drop = new Drop();
        EXPECT_TRUE(drop->isA(TypeId::C_DROP));
        EXPECT_EQ("Drop", drop->typeName());
    }

    TEST_F(Drop_Test, Eval) {
        VM* vm = new VM();
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Drop* drop = new Drop();
        vm->pushExpr(i300);
        vm->pushExpr(drop);
        vm->pushExpr(i200);
        vm->pushExpr(i100);
        while (vm->step());
        Object* res;
        EXPECT_TRUE(vm->popObj(res));
        EXPECT_EQ(i300, res);
        EXPECT_TRUE(vm->popObj(res));
        EXPECT_EQ(i100, res);
    }

}

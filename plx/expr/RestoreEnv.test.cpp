#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/RestoreEnv.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class RestoreEnv_Test : public PlxTestFixture {};

    TEST_F(RestoreEnv_Test, CreateInstance) {
        VM* vm = new VM();
        Triple* env = GLOBALS->EmptyTriple();
        RestoreEnv::pushContin(env, vm);
        Object* expr;
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_TRUE(expr->isA(TypeId::C_RESTOREENV));
        EXPECT_EQ("RestoreEnv", expr->typeName());
    }

    TEST_F(RestoreEnv_Test, Eval) {
        VM* vm = new VM();
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Triple* env = new Triple(x, i100);
        RestoreEnv::pushContin(env, vm);
        while (vm->step());
        EXPECT_EQ(env, vm->environment());
    }

    TEST_F(RestoreEnv_Test, Eval_TailCall) {
        VM* vm = new VM();
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Triple* env1 = new Triple(x, i100);
        RestoreEnv::pushContin(env1, vm);
        Identifier* y = Identifier::create("y");
        Integer* i200 = new Integer(200);
        Triple* env2 = new Triple(y, i200);
        RestoreEnv::pushContin(env2, vm);
        while (vm->step());
        EXPECT_EQ(env1, vm->environment());
    }

    TEST_F(RestoreEnv_Test, MarkChildren) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Triple* env = new Triple(x, i100);
        VM* vm = new VM();
        RestoreEnv::pushContin(env, vm);
        Object* expr;
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_TRUE(expr->isA(TypeId::C_RESTOREENV));
        RestoreEnv* restoreEnv = static_cast<RestoreEnv*>(expr);
        EXPECT_FALSE(restoreEnv->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(x->isMarked());
        std::vector<Object*> objs{restoreEnv};
        GC::mark(objs);
        EXPECT_FALSE(restoreEnv->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(x->isMarked());
    }

}

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Catch.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>

namespace PLX {

    class Catch_Test : public PlxTestFixture {};

    TEST_F(Catch_Test, CreateInstance) {
        Triple* rules = GLOBALS->EmptyTriple();
        Catch* catchExpr = new Catch(rules);
        EXPECT_TRUE(catchExpr->isA(TypeId::C_CATCH));
        EXPECT_EQ("CatchContin", catchExpr->typeName());
    }

    TEST_F(Catch_Test, Eval1) {
        Triple* rules = GLOBALS->EmptyTriple();
        Catch* catchExpr = new Catch(rules);
        VM* vm = new VM();
        catchExpr->eval(vm);
        Object* expr;
        ASSERT_FALSE(vm->popExpr(expr));
    }

    TEST_F(Catch_Test, Eval2) {
        Triple* rules = GLOBALS->EmptyTriple();
        Catch* catchExpr = new Catch(rules);
        VM* vm = new VM();
        vm->pushExpr(catchExpr);
        while (vm->step());
        Object* expr;
        ASSERT_FALSE(vm->popObj(expr));
        ASSERT_FALSE(vm->popExpr(expr));
    }

    TEST_F(Catch_Test, CatchException_Match1) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Integer* i400 = new Integer(400);
        Triple* rules = new Triple(i100, i200, new Triple(i300, i400));
        Catch* catchExpr = new Catch(rules);
        VM* vm = new VM();
        Object* bottomObj = new Object();
        vm->pushObj(bottomObj);
        Object* bottomExpr = new Object();
        vm->pushExpr(bottomExpr);
        Object* exception = i100;
        ASSERT_TRUE(catchExpr->catchException(exception, vm));
        Object* expr;
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_EQ(i200, expr);
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_EQ(bottomExpr, expr);
        ASSERT_TRUE(vm->popObj(expr));
        EXPECT_EQ(bottomObj, expr);
    }

    TEST_F(Catch_Test, CatchException_Match2) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Integer* i400 = new Integer(400);
        Triple* rules = new Triple(i100, i200, new Triple(i300, i400));
        Catch* catchExpr = new Catch(rules);
        VM* vm = new VM();
        Object* bottomObj = new Object();
        vm->pushObj(bottomObj);
        Object* bottomExpr = new Object();
        vm->pushExpr(bottomExpr);
        Object* exception = i300;
        ASSERT_TRUE(catchExpr->catchException(exception, vm));
        Object* expr;
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_EQ(i400, expr);
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_EQ(bottomExpr, expr);
        ASSERT_TRUE(vm->popObj(expr));
        EXPECT_EQ(bottomObj, expr);
    }

    TEST_F(Catch_Test, CatchException_NoMatch) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Integer* i400 = new Integer(400);
        Triple* rules = new Triple(i100, i200, new Triple(i300, i400));
        Catch* catchExpr = new Catch(rules);
        VM* vm = new VM();
        Object* bottomObj = new Object();
        vm->pushObj(bottomObj);
        Object* bottomExpr = new Object();
        vm->pushExpr(bottomExpr);
        Object* exception = GLOBALS->NilObject();
        ASSERT_FALSE(catchExpr->catchException(exception, vm));
        Object* expr;
        ASSERT_TRUE(vm->popExpr(expr));
        EXPECT_EQ(bottomExpr, expr);
        ASSERT_TRUE(vm->popObj(expr));
        EXPECT_EQ(bottomObj, expr);
    }

}

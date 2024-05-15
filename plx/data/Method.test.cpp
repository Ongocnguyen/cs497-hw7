#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/data/Method.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Method_Test : public PlxTestFixture {};

    TEST_F(Method_Test, CreateInstance) {
        Object* lhs = GLOBALS->EmptyList();
        Identifier* rhs = Identifier::create("length");
        Method* method1 = new Method(lhs, rhs);
        EXPECT_TRUE(method1->isA(TypeId::D_METHOD));
        EXPECT_EQ("Method", method1->typeName());
    }

    TEST_F(Method_Test, Apply) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = List::create({i100, i200, i300});
        Identifier* length = Identifier::create("length");
        Method* method1 = new Method(list1, length);
        VM* vm = new VM();
        method1->apply(vm, GLOBALS->EmptyList());
        while (vm->step());
        Object* res;
        ASSERT_TRUE(vm->popObj(res));
        Object* expectedRes = new Integer(3);
        EXPECT_EQ(*expectedRes, *res);
    }

    TEST_F(Method_Test, Eval) {
        Identifier* a = Identifier::create("a");
        Identifier* b = Identifier::create("b");
        Method* method1 = new Method(a, b);
        VM* vm = new VM();
        Object* res = vm->evalExpr(method1);
        EXPECT_EQ(method1, res);
    }

    TEST_F(Method_Test, FreeVars) {
        Identifier* a = Identifier::create("a");
        Identifier* b = Identifier::create("b");
        Method* method1 = new Method(a, b);
        List* freeVars = GLOBALS->EmptyList();
        freeVars = method1->freeVars(freeVars);
        int len;
        ASSERT_TRUE(freeVars->length(len));
        ASSERT_EQ(2, len);
        EXPECT_NE(GLOBALS->EmptyList(), freeVars->locate(a));
        EXPECT_NE(GLOBALS->EmptyList(), freeVars->locate(b));
    }

    TEST_F(Method_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Identifier* x = Identifier::create("x");
        Method* method1 = new Method(i100, x);
        EXPECT_FALSE(method1->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(x->isMarked());
        std::vector<Object*> objs{method1};
        GC::mark(objs);
        EXPECT_FALSE(method1->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(x->isMarked());
    }

    TEST_F(Method_Test, ShowOn) {
        Integer* i100 = new Integer(100);
        Identifier* x = Identifier::create("x");
        Method* method1 = new Method(i100, x);
        std::stringstream ss;
        ss << method1;
        EXPECT_EQ("100.x", ss.str());
    }

}

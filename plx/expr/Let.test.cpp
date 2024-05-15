#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/Let.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Let_Test : public PlxTestFixture {};

    TEST_F(Let_Test, CreateInstance) {
        Let* let1 = new Let(GLOBALS->EmptyTriple());
        EXPECT_TRUE(let1->isA(TypeId::E_LET));
        EXPECT_EQ("Let", let1->typeName());
    }

    TEST_F(Let_Test, Eval) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Identifier* y = Identifier::create("y");
        Integer* i200 = new Integer(200);
        Let* let1 = new Let(new Triple(x, i100, new Triple(y, i200)));
        VM* vm = new VM();
        Object* value = vm->evalExpr(let1);
        EXPECT_EQ(GLOBALS->NilObject(), value);
        value = vm->lookup(x);
        EXPECT_EQ(i100, value);
        value = vm->lookup(y);
        EXPECT_EQ(i200, value);
    }

    TEST_F(Let_Test, Eval_MatchFailure) {
        VM* vm = new VM();
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Let* let1 = new Let(new Triple(i100, i200));
        ASSERT_THROW(vm->evalExpr(let1), Array*);
    }

    TEST_F(Let_Test, MarkChildren) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Let* let1 = new Let(new Triple(x, i100));
        EXPECT_FALSE(let1->isMarked());
        EXPECT_FALSE(x->isMarked());
        EXPECT_FALSE(i100->isMarked());
        std::vector<Object*> objs{let1};
        GC::mark(objs);
        EXPECT_FALSE(let1->isMarked());
        EXPECT_TRUE(x->isMarked());
        EXPECT_TRUE(i100->isMarked());
    }

    TEST_F(Let_Test, ShowOn) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Let* let1 = new Let(new Triple(x, i100, new Triple(y, i200)));
        std::stringstream ss;
        ss << let1;
        EXPECT_EQ("let x=100, y=200", ss.str());
    }

}
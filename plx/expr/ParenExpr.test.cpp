#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/expr/Identifier.hpp>
#include <plx/expr/ParenExpr.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class ParenExpr_Test : public PlxTestFixture {};

    TEST_F(ParenExpr_Test, CreateInstance) {
        ParenExpr* parenExpr = new ParenExpr(GLOBALS->NilObject());
        EXPECT_TRUE(parenExpr->isA(TypeId::E_PARENEXPR));
        EXPECT_EQ("ParenExpr", parenExpr->typeName());
    }

    TEST_F(ParenExpr_Test, Eval) {
        VM* vm = new VM();
        Integer* i123 = new Integer(123);
        ParenExpr* parenExpr = new ParenExpr(i123);
        Object* value = vm->evalExpr(parenExpr);
        EXPECT_EQ(*i123, *value);
    }

    TEST_F(ParenExpr_Test, MarkChildren) {
        Integer* i123 = new Integer(123);
        ParenExpr* parenExpr = new ParenExpr(i123);
        EXPECT_FALSE(parenExpr->isMarked());
        EXPECT_FALSE(i123->isMarked());
        std::vector<Object*> objs{parenExpr};
        GC::mark(objs);
        EXPECT_FALSE(parenExpr->isMarked());
        EXPECT_TRUE(i123->isMarked());
    }

    TEST_F(ParenExpr_Test, ShowOn) {
        Integer* i123 = new Integer(123);
        ParenExpr* parenExpr = new ParenExpr(i123);
        std::stringstream ss;
        ss << parenExpr;
        EXPECT_EQ("(123)", ss.str());
    }

}

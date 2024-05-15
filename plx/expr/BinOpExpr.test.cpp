#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/BinOpExpr.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/String.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class BinOpExpr_Test : public PlxTestFixture {};

    TEST_F(BinOpExpr_Test, CreateInstance) {
        Integer* i100 = new Integer(100);
        Identifier* plus = Identifier::create("+");
        Integer* i200 = new Integer(200);
        BinOpExpr* binOpExpr = new BinOpExpr(i100, plus, i200);
        EXPECT_TRUE(binOpExpr->isA(TypeId::E_BINOPEXPR));
        EXPECT_EQ("BinOp", binOpExpr->typeName());
    }

    TEST_F(BinOpExpr_Test, Eval) {
        Integer* i100 = new Integer(100);
        Identifier* plus = Identifier::create("+");
        Integer* i200 = new Integer(200);
        BinOpExpr* binOpExpr = new BinOpExpr(i100, plus, i200);
        VM* vm = new VM();
        binOpExpr->eval(vm);
        while (vm->step());
        Object* result;
        ASSERT_TRUE(vm->popObj(result));
        Integer* expectedResult = new Integer(300);
        EXPECT_EQ(*expectedResult, *result);
    }

    TEST_F(BinOpExpr_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Identifier* plus = Identifier::create("+");
        Integer* i200 = new Integer(200);
        BinOpExpr* binOpExpr = new BinOpExpr(i100, plus, i200);
        EXPECT_FALSE(binOpExpr->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(plus->isMarked());
        EXPECT_FALSE(i200->isMarked());
        std::vector<Object*> objs{binOpExpr};
        GC::mark(objs);
        EXPECT_FALSE(binOpExpr->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(plus->isMarked());
        EXPECT_TRUE(i200->isMarked());
    }

    TEST_F(BinOpExpr_Test, ShowOn) {
        Integer* i100 = new Integer(100);
        Identifier* plus = Identifier::create("+");
        Integer* i200 = new Integer(200);
        BinOpExpr* binOpExpr = new BinOpExpr(i100, plus, i200);
        std::stringstream ss;
        ss << binOpExpr;
        EXPECT_EQ("100 + 200", ss.str());
    }

}

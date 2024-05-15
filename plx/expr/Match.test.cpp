#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/Match.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Match_Test : public PlxTestFixture {};

    TEST_F(Match_Test, CreateInstance) {
        Object* expr = GLOBALS->NilObject();
        Triple* rules = new Triple();
        Match* match = new Match(expr, rules);
        EXPECT_TRUE(match->isA(TypeId::E_MATCH));
        EXPECT_EQ("Match", match->typeName());
    }

    TEST_F(Match_Test, Eval_Match1) {
        Integer* i10 = new Integer(10);
        Integer* i100 = new Integer(100);
        Object* expression = i10;
        Triple* rules = new Triple(i10, i100);
        Match* match = new Match(expression, rules);
        VM* vm = new VM();
        Object* value = vm->evalExpr(match);
        ASSERT_EQ(*i100, *value);
    }

    TEST_F(Match_Test, Eval_Match2) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Object* expression = i20;
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        Match* match = new Match(expression, rules);
        VM* vm = new VM();
        Object* value = vm->evalExpr(match);
        ASSERT_EQ(*i200, *value);
    }

    TEST_F(Match_Test, Eval_MatchFail) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i30 = new Integer(30);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Object* expression = i30;
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        Match* match = new Match(expression, rules);
        VM* vm = new VM();
        ASSERT_THROW(vm->evalExpr(match), Array*);
    }

    TEST_F(Match_Test, MarkChildren) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i30 = new Integer(30);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Object* expression = i30;
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        Match* match = new Match(expression, rules);
        EXPECT_FALSE(match->isMarked());
        EXPECT_FALSE(expression->isMarked());
        EXPECT_FALSE(rules->isMarked());
        std::vector<Object*> objs{match};
        GC::mark(objs);
        EXPECT_FALSE(match->isMarked());
        EXPECT_TRUE(expression->isMarked());
        EXPECT_TRUE(rules->isMarked());
    }

    TEST_F(Match_Test, ShowOn) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i30 = new Integer(30);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Object* expression = i30;
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        Match* match = new Match(expression, rules);
        std::stringstream ss;
        ss << match;
        EXPECT_EQ("match 30 in 10=100 | 20=200 end", ss.str());
    }

}

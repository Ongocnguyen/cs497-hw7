#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Throw.hpp>
#include <plx/expr/TryCatch.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class TryCatch_Test : public PlxTestFixture {};

    TEST_F(TryCatch_Test, CreateInstance) {
        Integer* i10 = new Integer(10);
        TryCatch* tryCatch = new TryCatch(i10, GLOBALS->EmptyTriple());
        EXPECT_TRUE(tryCatch->isA(TypeId::E_TRYCATCH));
        EXPECT_EQ("TryCatch", tryCatch->typeName());
    }

    TEST_F(TryCatch_Test, Eval_NoThrow) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        TryCatch* tryCatch = new TryCatch(i10, GLOBALS->EmptyTriple());
        Object* value;
        ASSERT_NO_THROW(value = vm->evalExpr(tryCatch));
        EXPECT_EQ(i10, value);
    }

    TEST_F(TryCatch_Test, Eval_ThrowNoCatch) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        Throw* throwExpr = new Throw(i10);
        TryCatch* tryCatch = new TryCatch(throwExpr, GLOBALS->EmptyTriple());
        EXPECT_THROW(vm->evalExpr(tryCatch), Object*);
    }

    TEST_F(TryCatch_Test, Eval_ThrowCatch1) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        Integer* i100 = new Integer(100);
        Throw* throwExpr = new Throw(i10);
        Triple* rules = new Triple(i10, i100);
        TryCatch* tryCatch = new TryCatch(throwExpr, rules);
        Object* value;
        ASSERT_NO_THROW(value = vm->evalExpr(tryCatch));
        EXPECT_EQ(i100, value);
    }

    TEST_F(TryCatch_Test, Eval_ThrowCatch2) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Throw* throwExpr = new Throw(i20);
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        TryCatch* tryCatch = new TryCatch(throwExpr, rules);
        Object* value;
        ASSERT_NO_THROW(value = vm->evalExpr(tryCatch));
        EXPECT_EQ(i200, value);
    }

    class TryCatchTestObject : public Object {
    public:
        void eval(VM* vm) override {
            evalCalled++;
            vm->pushObj(this);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "TryCatchTestObject{" << evalCalled << '}';
        }
        int evalCalled = 0;
    };

    TEST_F(TryCatch_Test, Eval_NoThrowFinally) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        TryCatchTestObject* testObject = new TryCatchTestObject();
        TryCatch* tryCatch = new TryCatch(i10, GLOBALS->EmptyTriple(), testObject);
        Object* value;
        ASSERT_NO_THROW(value = vm->evalExpr(tryCatch));
        EXPECT_EQ(i10, value);
        EXPECT_EQ(1, testObject->evalCalled);
    }

    TEST_F(TryCatch_Test, Eval_ThrowCatchFinally) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        Integer* i100 = new Integer(100);
        Throw* throwExpr = new Throw(i10);
        Triple* rules = new Triple(i10, i100);
        TryCatchTestObject* testObject = new TryCatchTestObject();
        TryCatch* tryCatch = new TryCatch(throwExpr, rules, testObject);
        Object* value;
        ASSERT_NO_THROW(value = vm->evalExpr(tryCatch));
        EXPECT_EQ(i100, value);
        EXPECT_EQ(1, testObject->evalCalled);
    }

    TEST_F(TryCatch_Test, Eval_ThrowNoCatchFinally) {
        VM* vm = new VM();
        Integer* i10 = new Integer(10);
        Throw* throwExpr = new Throw(i10);
        Triple* rules = GLOBALS->EmptyTriple();
        TryCatchTestObject* finallyExpr = new TryCatchTestObject();
        TryCatch* tryCatch = new TryCatch(throwExpr, rules, finallyExpr);
        ASSERT_THROW(vm->evalExpr(tryCatch), Object*);
        EXPECT_EQ(1, finallyExpr->evalCalled);
    }

    TEST_F(TryCatch_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Integer* finally = new Integer(400);
        Throw* throwExpr = new Throw(i100);
        Triple* rules = new Triple(i200, i300);
        TryCatch* tryCatch = new TryCatch(throwExpr, rules, finally);
        EXPECT_FALSE(tryCatch->isMarked());
        EXPECT_FALSE(throwExpr->isMarked());
        EXPECT_FALSE(rules->isMarked());
        EXPECT_FALSE(finally->isMarked());
        std::vector<Object*> objs{tryCatch};
        GC::mark(objs);
        EXPECT_FALSE(tryCatch->isMarked());
        EXPECT_TRUE(throwExpr->isMarked());
        EXPECT_TRUE(rules->isMarked());
        EXPECT_TRUE(finally->isMarked());
    }

    TEST_F(TryCatch_Test, ShowOn) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Throw* throwExpr = new Throw(i10);
        Triple* rules = new Triple(i10, i100, new Triple(i20, i200));
        TryCatch* tryCatch = new TryCatch(throwExpr, rules, i300);
        std::stringstream ss;
        ss << tryCatch;
        EXPECT_EQ("try throw 10 catch 10=100 | 20=200 finally 300 end", ss.str());
    }

}
#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Do.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Do_Test : public PlxTestFixture {};

#if 0 // DO NOT DISABLE
    class DoTestObject : public Object {
    public:
        DoTestObject() {}
        void eval(VM* vm) override {
            (void)vm;
            this->seqValue = DoTestObject::seq++;
            vm->pushObj(this);
        }
        int seqValue;
        static int seq;
    };

    int DoTestObject::seq = 0;

    class DoTestObject2 : public Object {
    public:
        DoTestObject2() {}
        void eval(VM* vm) override {
            (void)vm;
            this->seqValue = DoTestObject::seq++;
            vm->pushObj(this);
        }
        int seqValue;
        static int seq;
    };

    TEST_F(Do_Test, CreateInstance) {
        Do* seq = new Do(GLOBALS->EmptyList());
        EXPECT_TRUE(seq->isA(TypeId::E_SEQ));
        EXPECT_EQ("Seq", seq->typeName());
    }

    TEST_F(Do_Test, Eval_Empty) {
        VM* vm = new VM();
        Do* seq = new Do(GLOBALS->EmptyList());
        Object* value = vm->evalExpr(seq);
        EXPECT_EQ(GLOBALS->NilObject(), value);
    }

    TEST_F(Do_Test, Eval_Nonempty) {
        VM* vm = new VM();
        DoTestObject::seq = 0;
        DoTestObject* obj1 = new DoTestObject();
        DoTestObject* obj2 = new DoTestObject();
        DoTestObject* obj3 = new DoTestObject();
        List* seqList = List::create({obj1, obj2, obj3});
        Do* seq = new Do(seqList);
        Object* value = vm->evalExpr(seq);
        EXPECT_EQ(value, obj3);
        EXPECT_EQ(0, obj1->seqValue);
        EXPECT_EQ(1, obj2->seqValue);
        EXPECT_EQ(2, obj3->seqValue);
    }

    class DoTestObject3 : public Object {
     public:
        DoTestObject3(Triple* initialEnv)
            : savedEnv {initialEnv}
        {}
        void eval(VM* vm) override {
            Triple* env = vm->environment();
            EXPECT_EQ(savedEnv, env);
            Object* key = new DoTestObject();
            Object* value = new DoTestObject();
            savedEnv = new Triple(key, value, env);
            vm->setEnvironment(savedEnv);
            vm->pushObj(GLOBALS->NilObject());
        }
        Triple* savedEnv;
    };

    TEST_F(Do_Test, Eval_PreservesEnvironment) {
        // this ensures that the VM's dynamic environment is preserved
        // between calls to subexpressions
        VM* vm = new VM();
        DoTestObject3* testObject = new DoTestObject3(vm->environment());
        List* elems = List::create({testObject, testObject, testObject});
        Do* doExpr = new Do(elems);
        Object* res = vm->evalExpr(doExpr);
        EXPECT_EQ(GLOBALS->NilObject(), res);
    }

    TEST_F(Do_Test, Eval_RestoresEnvironment) {
        VM* vm = new VM();
        DoTestObject2* expr = new DoTestObject2();
        Do* seq = new Do(new List(expr));
        Triple* savedEnv = vm->environment();
        vm->evalExpr(seq);
        EXPECT_EQ(savedEnv, vm->environment());
    }
#endif

    TEST_F(Do_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(100);
        Integer* i300 = new Integer(100);
        List* seqList = List::create({i100, i200, i300});
        Do* seq = new Do(seqList);
        EXPECT_FALSE(seq->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(i200->isMarked());
        EXPECT_FALSE(i300->isMarked());
        std::vector<Object*> objs{seq};
        GC::mark(objs);
        EXPECT_TRUE(seq->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(i200->isMarked());
        EXPECT_TRUE(i300->isMarked());
    }

#if 0 // DO NOT ENABLE
    TEST_F(Do_Test, ShowOn) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* seqList =
            new List(i100,
            new List(i200,
            new List(i300)));
        Do* seq = new Do(seqList);
        std::stringstream ss;
        ss << seq;
        EXPECT_EQ("do 100 200 300 end", ss.str());
    }
#endif

}

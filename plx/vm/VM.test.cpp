#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Catch.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class VM_Test : public PlxTestFixture {
    public:
        VM_Test()
            : _vm {new VM()}
        {}
        VM* _vm;
    };

    TEST_F(VM_Test, CreateInstance) {
        EXPECT_TRUE(_vm->isA(TypeId::T_VM));
        EXPECT_EQ("VM", _vm->typeName());
    }

    TEST_F(VM_Test, BindAndLookup) {
        Identifier* key1 = Identifier::create("abc");
        Integer* value1 = new Integer(100);
        _vm->bind(key1, value1);
        Object* value = _vm->lookup(key1);
        EXPECT_EQ(value1, value);
        Identifier* key2 = Identifier::create("def");
        EXPECT_THROW(_vm->lookup(key2), Array*);
    }

    TEST_F(VM_Test, PushObjAndPopObj) {
        Object* value;
        EXPECT_FALSE(_vm->popObj(value));
        Integer* i100 = new Integer(100);
        _vm->pushObj(i100);
        EXPECT_TRUE(_vm->popObj(value));
        EXPECT_EQ(i100, value);
    }

    TEST_F(VM_Test, PushExprAndPeekExpr) {
        Object* value;
        EXPECT_FALSE(_vm->popExpr(value));
        Integer* i100 = new Integer(100);
        _vm->pushExpr(i100);
        EXPECT_TRUE(_vm->peekExpr(value));
        EXPECT_EQ(i100, value);
    }

    TEST_F(VM_Test, PushExprAndPopExpr) {
        Object* value;
        EXPECT_FALSE(_vm->popExpr(value));
        Integer* i100 = new Integer(100);
        _vm->pushExpr(i100);
        EXPECT_TRUE(_vm->popExpr(value));
        EXPECT_EQ(i100, value);
    }

    TEST_F(VM_Test, PushExprAndPopExpr_WithEnv1) {
        Object* value;
        EXPECT_FALSE(_vm->popExpr(value));
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Identifier* x = Identifier::create("x");
        _vm->bind(x, i200);
        value = _vm->lookup(x);
        EXPECT_EQ(i200, value);
        _vm->pushExpr(i100);
        _vm->bind(x, i300);
        value = _vm->lookup(x);
        EXPECT_EQ(i300, value);
        EXPECT_TRUE(_vm->popExpr(value));
        EXPECT_EQ(i100, value);
        value = _vm->lookup(x);
        EXPECT_EQ(i200, value);
    }

    TEST_F(VM_Test, PushExprAndPopExpr_WithEnv2) {
        Object* value;
        EXPECT_FALSE(_vm->popExpr(value));
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Identifier* x = Identifier::create("x");
        Triple* newEnv = new Triple(x, i200, _vm->environment());
        _vm->pushExpr(i100, newEnv);
        _vm->bind(x, i300);
        value = _vm->lookup(x);
        EXPECT_EQ(i300, value);
        EXPECT_TRUE(_vm->popExpr(value));
        EXPECT_EQ(i100, value);
        value = _vm->lookup(x);
        EXPECT_EQ(i200, value);
    }

    class VMTestObject1 : public Object {
    public:
        VMTestObject1()
            : savedEnv {GLOBALS->EmptyTriple()}
        {}

        void eval(VM* vm) override {
            savedEnv = vm->environment();
            vm->pushObj(GLOBALS->NilObject());
        }
        Triple* savedEnv;
    };

    TEST_F(VM_Test, DynmicEnvironment) {
        // push an expression using DynamicEnvironment()
        // change the environment
        // evaluate the expression, the environment should be the new environment
        VM* vm = new VM();
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Triple* env1 = new Triple(x, i100, vm->environment());
        vm->setEnvironment(env1);
        VMTestObject1* testObject = new VMTestObject1();
        vm->pushExpr_dynamic(testObject);
        Identifier* y = Identifier::create("y");
        Integer* i200 = new Integer(200);
        Triple* env2 = new Triple(y, i200, vm->environment());
        vm->setEnvironment(env2);
        while (vm->step());
        EXPECT_EQ(env2, testObject->savedEnv);
    }

    // TEST_F(VM_Test, ExceptionThrown1) {
    //     EXPECT_FALSE(_vm->exceptionThrown(GLOBALS->NilObject()));
    // }

    // TEST_F(VM_Test, ExceptionThrown2) {
    //     EXPECT_FALSE(_vm->exceptionThrown(GLOBALS->NilObject()));
    // }

    TEST_F(VM_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Identifier* abc = Identifier::create("abc");
        _vm->bind(abc, i100);
        _vm->pushObj(i200);
        _vm->pushExpr(i300);
        EXPECT_FALSE(_vm->isMarked());
        EXPECT_FALSE(abc->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(i200->isMarked());
        EXPECT_FALSE(i300->isMarked());
        std::vector<Object*> objs{_vm};
        GC::mark(objs);
        EXPECT_FALSE(_vm->isMarked());
        EXPECT_TRUE(abc->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(i200->isMarked());
        EXPECT_TRUE(i300->isMarked());
    }

    class VMTestObject2 : public Object {
    public:
        VMTestObject2()
            : Object{}
            , evalCalled {0}
        {}

        void eval(VM* vm) override {
            evalCalled++;
            vm->pushObj(this);
        }

        int evalCalled;
    };

    TEST_F(VM_Test, Step) {
        EXPECT_FALSE(_vm->step());
        VMTestObject2* testObject = new VMTestObject2();
        _vm->pushExpr(testObject);
        EXPECT_TRUE(_vm->step());
        EXPECT_EQ(1, testObject->evalCalled);
    }

    TEST_F(VM_Test, Run) {
        EXPECT_THROW(_vm->run(), Array*);
        VMTestObject2* testObject = new VMTestObject2();
        _vm->pushExpr(testObject);
        Object* value = _vm->run();
        EXPECT_EQ(testObject, value);
    }

}

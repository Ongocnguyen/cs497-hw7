#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/vm/VM.hpp>
#include <plx/expr/Finally.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Finally_Test : public PlxTestFixture {};

    class FinallyTestObject : public Object {
    public:
        FinallyTestObject() {}
        void eval(VM* vm) override {
            (void)vm;
            this->seqValue = FinallyTestObject::seq++;
            vm->pushObj(this);
        }
        int seqValue;
        static int seq;
    };

    int FinallyTestObject::seq = 0;

    TEST_F(Finally_Test, CreateInstance) {
        FinallyTestObject* fto = new FinallyTestObject();
        Finally* finally = new Finally(fto);
        EXPECT_TRUE(finally->isA(TypeId::C_FINALLY));
        EXPECT_EQ("FinallyContin", finally->typeName());
    }

    TEST_F(Finally_Test, Eval) {
        VM* vm = new VM();
        FinallyTestObject* fto = new FinallyTestObject();
        Finally* finally = new Finally(fto);
        vm->pushExpr(finally);
        while (vm->step());
        Object* res;
        EXPECT_TRUE(vm->popObj(res));
        EXPECT_EQ(fto, res);
        EXPECT_EQ(1, fto->seq);
    }

}

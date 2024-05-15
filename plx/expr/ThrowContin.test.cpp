#include <gtest/gtest.h>

#include <plx/vm/VM.hpp>
#include <plx/expr/ThrowContin.hpp>
#include <tests/PlxTestFixture.hpp>

namespace PLX {

    class ThrowContin_Test : public PlxTestFixture {};

    TEST_F(ThrowContin_Test, CreateInstance) {
        ThrowContin* throwContin = new ThrowContin();
        ASSERT_TRUE(throwContin->isA(TypeId::C_THROWCONTIN));
        EXPECT_EQ("ThrowContin", throwContin->typeName());
    }

    class ThrowContinTestObject : public Object {
    public:
        ThrowContinTestObject() {}
        void eval(VM* vm) override {
            (void)vm;
            this->evalCalled++;
            vm->pushObj(this);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "ThrowContinTestObject{" << evalCalled << '}';
        }
        int evalCalled = 0;
    };

#if 0
    TEST_F(ThrowContin_Test, Eval1) {
        VM* vm = new VM();
        ThrowContinTestObject* testObj = new ThrowContinTestObject();
        vm->pushObj(testObj);
        ThrowContin* throwContin = new ThrowContin();
        throwContin->eval(vm);
        std::clog << vm << "\n";
        std::clog << throwContin << "\n";
        Object* expr;
        ASSERT_FALSE(vm->popExpr(expr));
    }
#endif

    // TODO add unit tests

}

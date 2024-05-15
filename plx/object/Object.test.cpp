#include <gtest/gtest.h>

#include <plx/gc/GC.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Object_Test : public ::testing::Test {
    public:
        void SetUp() override {
            _gc = new GC();
            Object::setGC(_gc);
        }
        void TearDown() override {
            delete _gc;
        }
    private:
        GC* _gc;
    };

    TEST_F(Object_Test, Equals) {
        Object* obj1 = new Object();
        EXPECT_TRUE(obj1->equals(obj1));
        Object* obj2 = new Object();
        EXPECT_FALSE(obj1->equals(obj2));
        EXPECT_FALSE(obj2->equals(obj1));
    }

    TEST_F(Object_Test, EqualOperator) {
        Object* obj1 = new Object();
        EXPECT_TRUE(*obj1 == *obj1);
        Object* obj2 = new Object();
        EXPECT_FALSE(*obj1 == *obj2);
        EXPECT_FALSE(*obj2 == *obj1);
        EXPECT_EQ(*obj1, *obj1);  // this requires equals() to use const
    }

}

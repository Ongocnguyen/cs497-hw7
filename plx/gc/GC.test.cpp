#include <gtest/gtest.h>

#include <plx/gc/GC.hpp>

namespace PLX {

    class TestObject : public Object {
    };

    TEST(GC_Test, CreateInstance) {
        GC gc;
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(0, gc.nRoots());
        EXPECT_EQ(0, gc.nRegistered());
    }

    TEST(GC_Test, Register) {
        GC gc;
        Object::setGC(&gc);
        EXPECT_EQ(0, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(0, gc.nRoots());
        Object* obj1 = new Object();
        EXPECT_EQ(1, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(0, gc.nRoots());
        gc.addRoot(obj1);
        EXPECT_EQ(1, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(1, gc.nRoots());
        gc.setRegistrationType(RegistrationType::PERMANENT);
        new Object();
        EXPECT_EQ(1, gc.nRegistered());
        EXPECT_EQ(1, gc.nPermanent());
        EXPECT_EQ(1, gc.nRoots());
    }

    TEST(GC_Test, MarkRoot) {
        GC gc;
        Object::setGC(&gc);
        Object* obj1 = new Object();
        gc.mark();
        EXPECT_FALSE(obj1->isMarked());
        gc.addRoot(obj1);
        gc.mark();
        EXPECT_TRUE(obj1->isMarked());
    }

    TEST(GC_Test, MarkAndSweep) {
        GC gc;
        Object::setGC(&gc);
        int nRegistered = gc.nRegistered();
        int nPermanent = gc.nPermanent();
        int nRoots = gc.nRoots();
        EXPECT_EQ(0, nPermanent);
        EXPECT_EQ(0, nRoots);
        Object* obj1 = new Object();
        EXPECT_EQ(++nRegistered, gc.nRegistered());
        Object* obj2 = new Object();
        EXPECT_EQ(++nRegistered, gc.nRegistered());
        gc.addRoot(obj2);
        EXPECT_EQ(++nRoots, gc.nRoots());
        gc.setRegistrationType(RegistrationType::PERMANENT);
        Object* obj3 = new Object();
        EXPECT_EQ(++nPermanent, gc.nPermanent());
        EXPECT_EQ(nRegistered, gc.nRegistered());
        gc.setRegistrationType(RegistrationType::REGULAR);
        EXPECT_FALSE(obj1->isMarked());
        EXPECT_FALSE(obj2->isMarked());
        EXPECT_FALSE(obj3->isMarked());
        gc.mark();
        EXPECT_FALSE(obj1->isMarked());
        EXPECT_TRUE(obj2->isMarked());
        EXPECT_FALSE(obj3->isMarked());
        gc.sweep();
        EXPECT_EQ(--nRegistered, gc.nRegistered());
        EXPECT_EQ(nRoots, gc.nRoots());
        EXPECT_EQ(nPermanent, gc.nPermanent());
        EXPECT_FALSE(obj2->isMarked());
        EXPECT_FALSE(obj3->isMarked());
    }

}

#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/InternTable.hpp>
#include <plx/gc/GC.hpp>
#include <plx/object/Globals.hpp>

namespace PLX {

    class InternTable_Test : public PlxTestFixture {};

    TEST_F(InternTable_Test, CreateInstance) {
        InternTable* it = new InternTable();
        EXPECT_EQ(TypeId::D_INTERNTABLE, it->typeId());
    }

    TEST_F(InternTable_Test, AddAndLookup) {
        InternTable* it = new InternTable();
        std::string key1 = "abc";
        std::string key2 = "def";
        Object* value1 = new Object();
        Object* value2 = new Object();
        Object* value;
        EXPECT_FALSE(it->lookup(key1, value));
        EXPECT_FALSE(it->lookup(key2, value));
        it->add(key1, value1);
        EXPECT_TRUE(it->lookup(key1, value));
        EXPECT_EQ(value1, value);
        EXPECT_FALSE(it->lookup(key2, value));
        it->add(key2, value2);
        EXPECT_TRUE(it->lookup(key1, value));
        EXPECT_EQ(value1, value);
        EXPECT_TRUE(it->lookup(key2, value));
        EXPECT_EQ(value2, value);
    }

#if 0
    TEST_F(InternTable_Test, GC) {
        GC* gcSaved = GLOBALS->Gc();
        GC gc;
        Object::setGC(&gc);
        EXPECT_EQ(0, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(0, gc.nRoots());

        InternTable* it = new InternTable();
        Object* obj1 = new Object();
        it->add("obj1", obj1);
        EXPECT_EQ(2, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(0, gc.nRoots());
        EXPECT_FALSE(it->isMarked());
        EXPECT_FALSE(obj1->isMarked());

        gc.mark();
        EXPECT_FALSE(it->isMarked());
        EXPECT_FALSE(obj1->isMarked());

        gc.addRoot(it);
        EXPECT_EQ(2, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(1, gc.nRoots());

        gc.mark();
        EXPECT_TRUE(it->isMarked());
        EXPECT_TRUE(obj1->isMarked());

        gc.sweep();
        EXPECT_FALSE(it->isMarked());
        EXPECT_FALSE(obj1->isMarked());
        EXPECT_EQ(2, gc.nRegistered());
        EXPECT_EQ(0, gc.nPermanent());
        EXPECT_EQ(1, gc.nRoots());
        Object::setGC(gcSaved);
    }
#endif

}

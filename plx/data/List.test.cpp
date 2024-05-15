#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class List_Test : public PlxTestFixture {};

    TEST_F(List_Test, CreateInstance) {
        List* empty = GLOBALS->EmptyList();
        EXPECT_TRUE(empty->isA(TypeId::D_LIST));
        EXPECT_EQ("List", empty->typeName());
        EXPECT_EQ(0, empty->length());
        EXPECT_THROW(empty->first(), Array*);
        EXPECT_THROW(empty->rest(), Array*);
        Integer* i100 = new Integer(100);
        List* list2 = new List(i100);
        EXPECT_TRUE(list2->isA(TypeId::D_LIST));
        EXPECT_EQ("List", list2->typeName());
        EXPECT_EQ(i100, list2->first());
        EXPECT_EQ(GLOBALS->EmptyList(), list2->rest());
    }

    TEST_F(List_Test, BoolValue) {
        List* list0 = GLOBALS->EmptyList();
        EXPECT_FALSE(list0->boolValue());
        List* list1 = new List(GLOBALS->NilObject(), list0);
        EXPECT_TRUE(list1->boolValue());
        List* list2 = new List(GLOBALS->NilObject(), list1);
        EXPECT_TRUE(list2->boolValue());
    }

    TEST_F(List_Test, Equals) {
        EXPECT_TRUE(GLOBALS->EmptyList()->equals(GLOBALS->EmptyList()));
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* list1 = new List(i100);
        List* list2 = new List(i100);
        EXPECT_TRUE(list1->equals(list2));
        EXPECT_TRUE(list2->equals(list1));
        List* list3 = new List(i200);
        EXPECT_FALSE(list3->equals(list1));
        EXPECT_FALSE(list1->equals(list3));
    }

    TEST_F(List_Test, Eval) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        VM* vm = new VM();
        vm->bind(x, i100);
        vm->bind(y, i200);
        List* list1 = new List(x, new List(y));
        Object* value = vm->evalExpr(list1);
        ASSERT_TRUE(value->isA(TypeId::D_LIST));
        List* listValue = static_cast<List*>(value);
        List* expectedList = new List(i100, new List(i200));
        EXPECT_TRUE(expectedList->equals(listValue));
    }

    TEST_F(List_Test, FreeVars) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        List* list1 = List::create({x, i100, y});
        List* freeVars = GLOBALS->EmptyList();
        freeVars = list1->freeVars(freeVars);
        int len;
        ASSERT_TRUE(freeVars->length(len));
        ASSERT_EQ(2, len);
        EXPECT_EQ(y, freeVars->first());
        EXPECT_EQ(x, freeVars->second());
    }

    TEST_F(List_Test, HashCode) {
        List* p0 = GLOBALS->EmptyList();
        HashCode hashCode0;
        EXPECT_TRUE(p0->hashCode(hashCode0));
        List* p1_1 = new List(new Integer(100), p0);
        List* p1_2 = new List(new Integer(100), p0);
        HashCode hashCode1_1;
        EXPECT_TRUE(p1_1->hashCode(hashCode1_1));
        HashCode hashCode1_2;
        EXPECT_TRUE(p1_2->hashCode(hashCode1_2));
        EXPECT_EQ(hashCode1_1, hashCode1_2);
        EXPECT_NE(hashCode0, hashCode1_1);
        EXPECT_NE(hashCode0, hashCode1_2);
    }

    TEST_F(List_Test, IsEmpty) {
        List* empty = GLOBALS->EmptyList();
        EXPECT_TRUE(empty->isEmpty());
        List* list1 = new List(GLOBALS->NilObject(), empty);
        EXPECT_FALSE(list1->isEmpty());
        List* list2 = new List(GLOBALS->NilObject(), empty);
        EXPECT_FALSE(list2->isEmpty());
    }

    TEST_F(List_Test, Length) {
        List* list0 = GLOBALS->EmptyList();
        EXPECT_EQ(0, list0->length());
        List* list1 = new List(GLOBALS->NilObject(), list0);
        EXPECT_EQ(1, list1->length());
        List* list2 = new List(GLOBALS->NilObject(), list1);
        EXPECT_EQ(2, list2->length());
    }

    TEST_F(List_Test, Locate) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = List::create({i100, i200});
        ASSERT_NE(GLOBALS->EmptyList(), list1->locate(i100));
        ASSERT_NE(GLOBALS->EmptyList(), list1->locate(i200));
        ASSERT_EQ(GLOBALS->EmptyList(), list1->locate(i300));
    }

    TEST_F(List_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* list1 = new List(i100);
        List* list2 = new List(i200, list1);
        EXPECT_FALSE(list2->isMarked());
        EXPECT_FALSE(list1->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(i200->isMarked());
        std::vector<Object*> objs{list2};
        GC::mark(objs);
        EXPECT_FALSE(list2->isMarked());
        EXPECT_TRUE(list1->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(i200->isMarked());
    }

    TEST_F(List_Test, Match) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* list1 = new List(i100, new List(i200));
        List* list2 = new List(x, new List(y));
        Triple* bindings = GLOBALS->EmptyTriple();
        EXPECT_TRUE(list2->match(list1, bindings));
        Object* value1;
        EXPECT_TRUE(bindings->lookup(x, value1));
        EXPECT_EQ(value1, i100);
        Object* value2;
        EXPECT_TRUE(bindings->lookup(y, value2));
        EXPECT_EQ(value2, i200);
    }

    TEST_F(List_Test, Match_Improper) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* list1 = new List(i100, new List(i200));
        List* list2 = new List(x, y);
        Triple* bindings = GLOBALS->EmptyTriple();
        EXPECT_TRUE(list2->match(list1, bindings));
        Object* value1;
        EXPECT_TRUE(bindings->lookup(x, value1));
        EXPECT_EQ(value1, i100);
        Object* value2;
        EXPECT_TRUE(bindings->lookup(y, value2));
        List* expectedList = new List(i200);
        EXPECT_EQ(*value2, *expectedList);
    }

    TEST_F(List_Test, MatchFail) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = new List(i100, new List(i200));
        List* list2 = new List(x, new List(i300));
        Triple* bindings = GLOBALS->EmptyTriple();
        EXPECT_FALSE(list2->match(list1, bindings));
    }

    TEST_F(List_Test, Reverse_ProperList) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = List::create({i100, i200, i300});
        List* reversed = list1->reverse();
        List* expected = List::create({i300, i200, i100});
        EXPECT_EQ(*expected, *reversed);
    }

    TEST_F(List_Test, Second) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = List::create({i100, i200, i300});
        EXPECT_EQ(i200, list1->second());
    }

    TEST_F(List_Test, SetGet) {
        List* list1 = new List(GLOBALS->NilObject());
        EXPECT_EQ(GLOBALS->NilObject(), list1->first());
        EXPECT_EQ(GLOBALS->EmptyList(), list1->rest());
        Integer* i100 = new Integer(100);
        list1->setFirst(i100);
        EXPECT_EQ(i100, list1->first());
        Integer* i200 = new Integer(200);
        List* list2 = new List(i200);
        list1->setRest(list2);
        EXPECT_EQ(list2, list1->rest());
    }

    TEST_F(List_Test, ShowOn) {
        {
            std::stringstream ss;
            ss << GLOBALS->EmptyList();
            EXPECT_EQ("[]", ss.str());
        }
        Integer* i100 = new Integer(100);
        List* list1 = new List(i100);
        {
            std::stringstream ss;
            ss << list1;
            EXPECT_EQ("[100]", ss.str());
        }
        Integer* i200 = new Integer(200);
        List* list2 = new List(i200, list1);
        {
            std::stringstream ss;
            ss << list2;
            EXPECT_EQ("[200, 100]", ss.str());
        }
    }

    TEST_F(List_Test, ToArray) {
        List* list0 = GLOBALS->EmptyList();
        Array* array0;
        EXPECT_TRUE(list0->toArray(array0));
        Array* expectedArray0 = new Array({});
        EXPECT_EQ(*expectedArray0, *array0);

        Integer* i100 = new Integer(100);
        List* list1 = new List(i100);
        Array* array1;
        EXPECT_TRUE(list1->toArray(array1));
        Array* expectedArray1 = new Array({i100});
        EXPECT_EQ(*expectedArray1, *array1);

        Integer* i200 = new Integer(200);
        List* list2 = new List(i200, list1);
        Array* array2;
        EXPECT_TRUE(list2->toArray(array2));
        Array* expectedArray2 = new Array({i200, i100});
        EXPECT_EQ(*expectedArray2, *array2);
    }

    TEST_F(List_Test, ToQueue) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        List* list1 = List::create({i100, i200, i300});
        Queue* queue1;
        EXPECT_TRUE(list1->toQueue(queue1));
        Queue* expectedQueue = new Queue({i100, i200, i300});
        EXPECT_EQ(*expectedQueue, *queue1);
    }
}

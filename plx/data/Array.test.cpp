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
#include <plx/vm/VM.hpp>

namespace PLX {

    class Array_Test : public PlxTestFixture {};

    TEST_F(Array_Test, CreateInstance) {
        Array* a3 = new Array(3);
        EXPECT_TRUE(a3->isA(TypeId::D_ARRAY));
        EXPECT_EQ("Array", a3->typeName());
        EXPECT_EQ(3, a3->length());
        for (int n=0; n<3; n++) {
            Object* elem;
            EXPECT_TRUE(a3->get(n, elem));
            EXPECT_EQ(GLOBALS->NilObject(), elem);
        }
    }

    TEST_F(Array_Test, CreateFromList) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* a3 = new Array({i100, i200, i300});
        EXPECT_EQ(3, a3->length());
        Object* elem;
        EXPECT_TRUE(a3->get(0, elem));
        EXPECT_EQ(i100, elem);
        EXPECT_TRUE(a3->get(1, elem));
        EXPECT_EQ(i200, elem);
        EXPECT_TRUE(a3->get(2, elem));
        EXPECT_EQ(i300, elem);
    }

    TEST_F(Array_Test, BoolValue) {
        Array* a0 = new Array(0);
        Array* a1 = new Array(1);
        EXPECT_FALSE(a0->boolValue());
        EXPECT_TRUE(a1->boolValue());
    }

    TEST_F(Array_Test, Equals) {
        Array* a3a = new Array(3);
        Array* a3b = new Array(3);
        Array* a3c = new Array(3);
        Array* a4 = new Array(4);
        a3c->set(2, new Integer(123));
        EXPECT_TRUE(a3a->equals(a3b));
        EXPECT_EQ(*a3a, *a3b);
        EXPECT_TRUE(a3b->equals(a3a));
        EXPECT_EQ(*a3b, *a3a);
        EXPECT_FALSE(a3a->equals(a3c));
        EXPECT_NE(*a3a, *a3c);
        EXPECT_FALSE(a3c->equals(a4));
        EXPECT_NE(*a3c, *a4);
    }

    TEST_F(Array_Test, Eval) {
        GC* savedGc = GLOBALS->Gc();
        GC gc;
        Object::setGC(&gc);
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Identifier* z = Identifier::create("z");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        VM* vm = new VM();
        gc.addRoot(vm);
        vm->bind(x, i100);
        vm->bind(y, i200);
        vm->bind(z, i300);
        Array* array1 = new Array({x, y, z});
        Object* resObj = vm->evalExpr(array1);
        ASSERT_TRUE(resObj->isA(TypeId::D_ARRAY));
        Array* resArray = static_cast<Array*>(resObj);
        Array* expectedArray = new Array({i100, i200, i300});
        EXPECT_EQ(*expectedArray, *resArray);
        Object::setGC(savedGc);
    }

    TEST_F(Array_Test, EvalUnboundIdentifier) {
        GC* savedGc = GLOBALS->Gc();
        GC gc;
        Object::setGC(&gc);
        Identifier* x = Identifier::create("x");
        std::clog << "Array_Test(1) x = " << x << "\n";
        Array* array1 = new Array({x});
        std::clog << "Array_Test(1) array1 = " << array1 << "\n";
        VM* vm = new VM();
        gc.addRoot(vm);
        // check that the value is an error array
        std::clog << "Array_Test(1) array1 = " << array1 << "\n";
        EXPECT_THROW(vm->evalExpr(array1), Array*);
        std::clog << "Array_Test(2) array1 = " << array1 << "\n";
        Object::setGC(savedGc);
    }

    TEST_F(Array_Test, FreeVars) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Array* array1 = new Array({x, i100, y});
        List* freeVars = GLOBALS->EmptyList();
        freeVars = array1->freeVars(freeVars);
        int len;
        ASSERT_TRUE(freeVars->length(len));
        ASSERT_EQ(2, len);
        EXPECT_EQ(y, freeVars->first());
        EXPECT_EQ(x, freeVars->second());
    }

    TEST_F(Array_Test, HashCode) {
        Integer* i100_1 = new Integer(100);
        Integer* i100_2 = new Integer(100);
        Array* a3_1 = new Array(3);
        a3_1->set(1, i100_1);
        Array* a3_2 = new Array(3);
        a3_2->set(1, i100_2);
        HashCode hashCode1;
        EXPECT_TRUE(a3_1->hashCode(hashCode1));
        HashCode hashCode2;
        EXPECT_TRUE(a3_2->hashCode(hashCode2));
        EXPECT_EQ(hashCode1, hashCode2);
        Array* a0 = new Array(0);
        HashCode hashCode3;
        EXPECT_TRUE(a0->hashCode(hashCode3));
        EXPECT_NE(hashCode1, hashCode3);
        EXPECT_NE(hashCode2, hashCode3);
    }

    TEST_F(Array_Test, Index) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* a3 = new Array({i100, i200, i300});
        Object* value;
        ASSERT_TRUE(a3->index(new Integer(0), value));
        EXPECT_EQ(i100, value);
        ASSERT_TRUE(a3->index(new Integer(1), value));
        EXPECT_EQ(i200, value);
        ASSERT_TRUE(a3->index(new Integer(2), value));
        EXPECT_EQ(i300, value);
        EXPECT_THROW(a3->index(new Integer(-1), value), Array*);
        EXPECT_THROW(a3->index(new Integer(3), value), Array*);
    }

    TEST_F(Array_Test, Length) {
        Array* a0 = new Array(0);
        Array* a1 = new Array(1);
        Array* a2 = new Array(2);
        EXPECT_EQ(0, a0->length());
        EXPECT_EQ(1, a1->length());
        EXPECT_EQ(2, a2->length());
    }

    TEST_F(Array_Test, MarkChildren) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* a3 = new Array({i100, i200, i300});
        EXPECT_FALSE(a3->isMarked());
        EXPECT_FALSE(i100->isMarked());
        EXPECT_FALSE(i200->isMarked());
        EXPECT_FALSE(i300->isMarked());
        std::vector<Object*> objs{a3};
        GC::mark(objs);
        EXPECT_FALSE(a3->isMarked());
        EXPECT_TRUE(i100->isMarked());
        EXPECT_TRUE(i200->isMarked());
        EXPECT_TRUE(i300->isMarked());
    }

    TEST_F(Array_Test, Match) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* array1 = new Array({i100, i200, i300});
        Array* array2 = new Array({x, i200, y});
        Triple* bindings = GLOBALS->EmptyTriple();
        EXPECT_TRUE(array2->match(array1, bindings));
        Object* value1;
        EXPECT_TRUE(bindings->lookup(x, value1));
        EXPECT_EQ(value1, i100);
        Object* value2;
        EXPECT_TRUE(bindings->lookup(y, value2));
        EXPECT_EQ(value2, i300);
    }

    TEST_F(Array_Test, MatchFail) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* array1 = new Array({i100, i200, i300});
        Array* array2 = new Array({x, i300, y});
        Triple* bindings = GLOBALS->EmptyTriple();
        EXPECT_FALSE(array2->match(array1, bindings));
    }

    TEST_F(Array_Test, SetGet) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* a3 = new Array({i100, i200, i300});
        Object* elem;
        EXPECT_FALSE(a3->get(-1, elem));
        EXPECT_TRUE(a3->get(0, elem));
        EXPECT_EQ(i100, elem);
        EXPECT_TRUE(a3->get(1, elem));
        EXPECT_EQ(i200, elem);
        EXPECT_TRUE(a3->get(2, elem));
        EXPECT_EQ(i300, elem);
        EXPECT_FALSE(a3->get(3, elem));
    }

    TEST_F(Array_Test, GetOutOfBounds) {
        Array* array = new Array(0);
        Object* value;
        EXPECT_FALSE(array->get(0, value));
        // check that the value is an error array
        EXPECT_TRUE(value->isA(TypeId::D_ARRAY));
        Array* errorArray = static_cast<Array*>(value);
        EXPECT_EQ(3, errorArray->length());
        Object* elem;
        EXPECT_TRUE(errorArray->get(0, elem));
        EXPECT_TRUE(elem->isA(TypeId::L_STRING));
    }

    TEST_F(Array_Test, ShowOn) {
        {
            std::stringstream ss;
            ss << GLOBALS->NilObject();
            EXPECT_EQ("nil", ss.str());
        }
        {
            Array* a0 = new Array(0);
            std::stringstream ss;
            ss << a0;
            EXPECT_EQ("{}", ss.str());
        }
        {
            Array* a1 = new Array(1);
            std::stringstream ss;
            ss << a1;
            EXPECT_EQ("{nil}", ss.str());
        }
        {
            Array* a2 = new Array(2);
            std::stringstream ss;
            ss << a2;
            EXPECT_EQ("{nil, nil}", ss.str());
        }
    }

    TEST_F(Array_Test, ToList) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* array = new Array({i100, i200, i300});
        List* list1;
        EXPECT_TRUE(array->toList(list1));
        List* expectedList = List::create({i100, i200, i300});
        EXPECT_EQ(*expectedList, *list1);
    }

    TEST_F(Array_Test, ToQueue) {
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        Integer* i300 = new Integer(300);
        Array* array = new Array({i100, i200, i300});
        Queue* queue1;
        EXPECT_TRUE(array->toQueue(queue1));
        Queue* expectedQueue = new Queue({i100, i200, i300});
        EXPECT_EQ(*expectedQueue, *queue1);
    }

}

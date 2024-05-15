#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class Function_Test : public PlxTestFixture {};

    TEST_F(Function_Test, CreateInstance) {
        List* parameters = GLOBALS->EmptyList();
        Object* body = GLOBALS->NilObject();
        Function* fun1 = new Function(parameters, body);
        EXPECT_TRUE(fun1->isA(TypeId::E_FUNCTION));
        EXPECT_EQ("Function", fun1->typeName());
    }

    TEST_F(Function_Test, Eval) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* parameters = new List(x);
        Object* body = new Array({x, y});
        Function* fun1 = new Function(parameters, body);
        VM* vm = new VM();
        vm->bind(y, i200);
        Object* value1 = vm->evalExpr(fun1);
        ASSERT_TRUE(value1->isA(TypeId::D_CLOSURE));
        // the only way to test if the closure was created correctly
        // is to apply it to arguments
        Closure* closure1 = static_cast<Closure*>(value1);
        List* args = new List(i100);
        closure1->apply(vm, args);
        Object* value2 = vm->run();
        Array* expectedArray = new Array({i100, i200});
        EXPECT_TRUE(value2->equals(expectedArray));
    }

    TEST_F(Function_Test, MarkChildren) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        List* parameters = new List(x);
        Object* body = y;
        Function* fun1 = new Function(parameters, body);
        EXPECT_FALSE(fun1->isMarked());
        EXPECT_FALSE(x->isMarked());
        EXPECT_FALSE(y->isMarked());
        EXPECT_FALSE(parameters->isMarked());
        std::vector<Object*> objs{fun1};
        GC::mark(objs);
        EXPECT_FALSE(fun1->isMarked());
        EXPECT_TRUE(x->isMarked());
        EXPECT_TRUE(y->isMarked());
        EXPECT_TRUE(parameters->isMarked());
    }

    TEST_F(Function_Test, ShowOn) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        List* parameters = new List(x);
        Object* body = y;
        Function* fun1 = new Function(parameters, body);
        std::stringstream ss;
        ss << fun1;
        EXPECT_EQ("fun (x) = y", ss.str());
    }

    TEST_F(Function_Test, MatchArgumentsToParameters) {
        Integer* i10 = new Integer(10);
        Integer* i20 = new Integer(20);
        Integer* i30 = new Integer(30);
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* paramList1 = List::create({i10});
        Object* body1 = i100;
        List* paramList2 = List::create({i20});
        Object* body2 = i200;
        Function* function1 = new Function(paramList1, body1);
        Function* function2 = new Function(paramList2, body2);
        function1->setNext(function2);
        Triple* lexicalEnv = GLOBALS->EmptyTriple();
        List* argList1 = List::create({i10});
        Object* bodyRes1 = function1->matchArgumentsToParameters(argList1, lexicalEnv);
        EXPECT_EQ(*body1, *bodyRes1);
        List* argList2 = List::create({i20});
        Object* bodyRes2 = function1->matchArgumentsToParameters(argList2, lexicalEnv);
        EXPECT_EQ(*body2, *bodyRes2);
        List* argList3 = List::create({i30});
        EXPECT_THROW(function1->matchArgumentsToParameters(argList3, lexicalEnv), Array*);
    }

}

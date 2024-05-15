#include <gtest/gtest.h>

#include <tests/PlxTestFixture.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/gc/GC.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>

namespace PLX {

    class Closure_Test : public PlxTestFixture {};

    TEST_F(Closure_Test, CreateInstance) {
        List* parameters = GLOBALS->EmptyList();
        Object* body = GLOBALS->NilObject();
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = GLOBALS->EmptyTriple();
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        EXPECT_TRUE(closure1->isA(TypeId::D_CLOSURE));
        EXPECT_EQ("Closure", closure1->typeName());
    }

    TEST_F(Closure_Test, Apply) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* parameters = new List(x, new List(y));
        Object* body = new Array({x, y});
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = GLOBALS->EmptyTriple();
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        VM* vm = new VM();
        List* arguments = new List(i100, new List(i200));
        closure1->apply(vm, arguments);
        while (vm->step());
        Array* expectedArray = new Array({i100, i200});
        Object* actualValue;
        ASSERT_TRUE(vm->popObj(actualValue));
        EXPECT_TRUE(expectedArray->equals(actualValue));
    }

#if 0
    TEST_F(Closure_Test, Apply_Macro) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Identifier* a = Identifier::create("a");
        Identifier* b = Identifier::create("b");
        List* parameters = new List(x, new List(y));
        Object* body = new Array({x, y});
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = GLOBALS->EmptyTriple();
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        VM* vm = new VM();
        List* arguments = new List(a, new List(b));
        Object* value = closure1->apply(vm, arguments);
        ASSERT_TRUE(value->isA(TypeId::D_ARRAY));
        Array* valueArray = static_cast<Array*>(value);
        Array* expectedArray = new Array({a, b});
        EXPECT_TRUE(expectedArray->equals(valueArray));
    }
#endif

    TEST_F(Closure_Test, Apply_LexicalEnvironment) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        Integer* i200 = new Integer(200);
        List* parameters = GLOBALS->EmptyList();
        Object* body = x;
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = new Triple(x, i100);
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        VM* vm = new VM();
        // The lexical environment has x=100 and the dynamic environment has x=200.
        // The lexical environment should be used before the dynamic environment.
        vm->bind(x, i200);
        List* arguments = GLOBALS->EmptyList();
        closure1->apply(vm, arguments);
        while (vm->step());
        Object* value;
        ASSERT_TRUE(vm->popObj(value));
        ASSERT_TRUE(value->isA(TypeId::L_INTEGER));
        EXPECT_TRUE(i100->equals(value));
    }

    TEST_F(Closure_Test, Apply_DynamicEnvironment) {
        Identifier* x = Identifier::create("x");
        Integer* i100 = new Integer(100);
        List* parameters = GLOBALS->EmptyList();
        Object* body = x;
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = GLOBALS->EmptyTriple();
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        VM* vm = new VM();
        // The lexical environment is empty and the dynamic environment has x=200.
        // The dynamic environment is not visible to this expression, though.
        vm->bind(x, i100);
        List* arguments = GLOBALS->EmptyList();
        closure1->apply(vm, arguments);
        auto lam = [vm]() { while (vm->step()); };
        EXPECT_THROW(lam(), Array*);
    }

    TEST_F(Closure_Test, MarkChildren) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Identifier* z = Identifier::create("z");
        Integer* i100 = new Integer(100);
        List* parameters = new List(x);
        Object* body = y;
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = new Triple(z, i100);
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        EXPECT_FALSE(closure1->isMarked());
        EXPECT_FALSE(function->isMarked());
        EXPECT_FALSE(lexicalEnvironment->isMarked());
        std::vector<Object*> objs{closure1};
        GC::mark(objs);
        EXPECT_FALSE(closure1->isMarked());
        EXPECT_TRUE(function->isMarked());
        EXPECT_TRUE(lexicalEnvironment->isMarked());
    }

    TEST_F(Closure_Test, ShowOn) {
        Identifier* x = Identifier::create("x");
        Identifier* y = Identifier::create("y");
        Identifier* z = Identifier::create("z");
        Integer* i100 = new Integer(100);
        List* parameters = new List(x);
        Object* body = y;
        Function* function = new Function(parameters, body);
        Triple* lexicalEnvironment = new Triple(z, i100);
        Closure* closure1 = new Closure(function, lexicalEnvironment);
        std::stringstream ss;
        ss << closure1;
        EXPECT_EQ("fun (x) = y", ss.str());
    }

}

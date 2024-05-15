#include <plx/literal/PrimitiveFunction.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Array.hpp>
#include <plx/prim/Data.hpp>
#include <plx/prim/Global.hpp>
#include <plx/prim/HashTable.hpp>
#include <plx/prim/IO.hpp>
#include <plx/prim/List.hpp>
#include <plx/prim/Operator.hpp>
#include <plx/prim/Queue.hpp>

namespace PLX {

    void definePrim(VM* vm, const std::string& name, Primitive prim) {
        vm->bind(Identifier::create(name), new PrimitiveFunction(name, prim));
    }

    void definePrims(VM* vm) {
        // Prim_Global
        definePrim(vm, "argMap", Prim_Global::argMap);
        definePrim(vm, "close", Prim_Global::close);
        definePrim(vm, "eval", Prim_Global::eval);
        definePrim(vm, "gc", Prim_Global::gc);
        definePrim(vm, "not", Prim_Global::notOperator);
        definePrim(vm, "repl", Prim_Global::repl);
        definePrim(vm, "tokenize", Prim_Global::tokenize);
        definePrim(vm, "type", Prim_Global::type);
        // Prim_Array
        definePrim(vm, "get", Prim_Array::get);
        definePrim(vm, "set", Prim_Array::set);
        // Prim_Data
        definePrim(vm, "length", Prim_Data::length);
        definePrim(vm, "toArray", Prim_Data::toArray);
        definePrim(vm, "toList", Prim_Data::toList);
        definePrim(vm, "toQueue", Prim_Data::toQueue);
        definePrim(vm, "toString", Prim_Data::toString);
        // Prim_HashTable
        definePrim(vm, "keys", Prim_HashTable::keys);
        // Prim_IO
        definePrim(vm, "display", Prim_IO::display);
        definePrim(vm, "nl", Prim_IO::nl);
        definePrim(vm, "readLine", Prim_IO::readLine);
        definePrim(vm, "show", Prim_IO::show);
        // Prim_List
        definePrim(vm, "first", Prim_List::first);
        definePrim(vm, "rest", Prim_List::rest);
        // Prim_Queue
        definePrim(vm, "enq", Prim_Queue::enq);
        definePrim(vm, "deq", Prim_Queue::deq);
        // Prim_Operator
        definePrim(vm, "=", Prim_Operator::bind);
        definePrim(vm, "==", Prim_Operator::equalTo);
        definePrim(vm, "+", Prim_Operator::plus);
        definePrim(vm, "-", Prim_Operator::minus);
        definePrim(vm, "*", Prim_Operator::times);
        definePrim(vm, "/", Prim_Operator::divide);
        definePrim(vm, ".", Prim_Operator::dot);
        definePrim(vm, "%", Prim_Operator::percent);
    }

}

#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_Array {

        void get(VM* vm, List* args) {
            static const std::string PRIM_NAME = "get";
            checkArgTypes(PRIM_NAME, args, {TypeId::D_ARRAY, TypeId::L_INTEGER});
            Array* array = static_cast<Array*>(args->first());
            Integer* index = static_cast<Integer*>(args->second());
            Object* value;
            assert(array->index(index, value));
            vm->pushObj(value);
        }

        void set(VM* vm, List* args) {
            static const std::string PRIM_NAME = "set";
            checkArgTypes(PRIM_NAME, args, {TypeId::D_ARRAY, TypeId::L_INTEGER, TypeId::Z_ANY});
            Array* array = static_cast<Array*>(args->first());
            Integer* index = static_cast<Integer*>(args->second());
            Object* value = args->third();
            array->set(index->value(), value);
            vm->pushObj(GLOBALS->NilObject());
        }

    }
}

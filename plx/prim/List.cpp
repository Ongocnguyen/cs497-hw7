#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_List {

        void first(VM* vm, List* args) {
            checkArgTypes("first", args, {TypeId::D_LIST});
            List* list = static_cast<List*>(args->first());
            vm->pushObj(list->first());
        }

        void rest(VM* vm, List* args) {
            checkArgTypes("rest", args, {TypeId::D_LIST});
            List* list = static_cast<List*>(args->first());
            vm->pushObj(list->rest());
        }

        void setFirst(VM* vm, List* args) {
            checkArgTypes("setFirst", args, {TypeId::D_LIST, TypeId::Z_ANY});
            List* list = static_cast<List*>(args->first());
            Object* elem = args->second();
            list->setFirst(elem);
            vm->pushObj(list);
        }

        void setRest(VM* vm, List* args) {
            checkArgTypes("setRest", args, {TypeId::D_LIST, TypeId::Z_ANY});
            List* list = static_cast<List*>(args->first());
            Object* elem = args->second();
            list->setRest(elem);
            vm->pushObj(list);
        }

    }
}

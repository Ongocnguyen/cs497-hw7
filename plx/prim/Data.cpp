#include <cassert>
#include <sstream>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_Data {

        void length(VM* vm, List* args) {
            static const std::string PRIM_NAME = "length";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* elem = args->first();
            int len;
            if (!elem->length(len)) {
                throwException(PRIM_NAME, "Illegal argument type",
                    new Array({new Triple(Symbol::create("Value"), elem),
                               new Triple(Symbol::create("Type"), Symbol::create(elem->typeName()))}));
            }
            vm->pushObj(new Integer(len));
        }

        void toArray(VM* vm, List* args) {
            static const std::string PRIM_NAME = "toArray";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* elem = args->first();
            Array* array;
            if (!elem->toArray(array)) {
                throwException(PRIM_NAME, "Illegal argument type",
                    new Array({new Triple(Symbol::create("Value"), elem),
                               new Triple(Symbol::create("Type"), Symbol::create(elem->typeName()))}));
            }
            vm->pushObj(array);
        }

        void toList(VM* vm, List* args) {
            static const std::string PRIM_NAME = "toList";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* elem = args->first();
            List* list;
            if (!elem->toList(list)) {
                throwException(PRIM_NAME, "Illegal argument type",
                    new Array({new Triple(Symbol::create("Value"), elem),
                               new Triple(Symbol::create("Type"), Symbol::create(elem->typeName()))}));
            }
            vm->pushObj(list);
        }

        void toQueue(VM* vm, List* args) {
            static const std::string PRIM_NAME = "toQueue";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* elem = args->first();
            Queue* queue;
            if (!elem->toQueue(queue)) {
                throwException(PRIM_NAME, "Illegal argument type",
                    new Array({new Triple(Symbol::create("Value"), elem),
                               new Triple(Symbol::create("Type"), Symbol::create(elem->typeName()))}));
            }
            vm->pushObj(queue);
        }

        void toString(VM* vm, List* args) {
            static const std::string PRIM_NAME = "toString";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* arg = args->first();
            std::ostringstream oss;
            oss << arg;
            String* string = new String(oss.str());
            vm->pushObj(string);
        }

    }
}

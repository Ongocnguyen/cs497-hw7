#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_HashTable {

        void keys(VM* vm, List* args) {
            checkArgTypes("keys", args, {TypeId::D_HASHTABLE});
            HashTable* hashTable = static_cast<HashTable*>(args->first());
            Array* keys = hashTable->keys();
            vm->pushObj(keys);
        }

    }
}

#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_Queue {

        void enq(VM* vm, List* args) {
            checkArgTypes("enq", args, {TypeId::D_QUEUE, TypeId::Z_ANY});
            Queue* queue = static_cast<Queue*>(args->first());
            Object* elem = args->second();
            queue->enq(elem);
            vm->pushObj(queue);
        }

        void deq(VM* vm, List* args) {
            checkArgTypes("deq", args, {TypeId::D_QUEUE});
            Queue* queue = static_cast<Queue*>(args->first());
            vm->pushObj(queue->deq());
        }

    }
}

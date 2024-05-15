namespace PLX {

    class VM;
    class List;
    class Object;

    namespace Prim_Queue {

        void enq(VM* vm, List* args);
        void deq(VM* vm, List* args);

    }
}

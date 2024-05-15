
namespace PLX {

    class List;
    class Object;
    class VM;

    namespace Prim_Data {

        void length(VM* vm, List* args);
        void toArray(VM* vm, List* args);
        void toList(VM* vm, List* args);
        void toQueue(VM* vm, List* args);
        void toString(VM* vm, List* args);

    }
}

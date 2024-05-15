
namespace PLX {

    class VM;
    class List;
    class Object;

    namespace Prim_List {

        void asArray(VM* vm, List* args);
        void first(VM* vm, List* args);
        void rest(VM* vm, List* args);
        void setFirst(VM* vm, List* args);
        void setRest(VM* vm, List* args);

    }
}

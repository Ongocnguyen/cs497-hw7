namespace PLX {

    class VM;
    class List;
    class Object;

    namespace Prim_IO {

        void display(VM* vm, List* args);
        void nl(VM* vm, List* args);
        void readLine(VM* vm, List* args);
        void show(VM* vm, List* args);

    }
}

namespace PLX {

    class VM;
    class List;
    class Object;

    namespace Prim_Operator {

        void bind(VM* vm, List* args);
        void divide(VM* vm, List* args);
        void dot(VM* vm, List* args);
        void equalTo(VM* vm, List* args);
        void minus(VM* vm, List* args);
        void plus(VM* vm, List* args);
        void times(VM* vm, List* args);
        void percent(VM* vm, List* args);

    }
}

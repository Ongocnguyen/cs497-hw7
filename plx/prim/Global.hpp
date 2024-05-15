namespace PLX {

    class List;
    class Object;
    class VM;

    namespace Prim_Global {
        void argMap(VM* vm, List* args);
        void close(VM* vm, List* args);
        void eval(VM* vm, List* args);
        void gc(VM* vm, List* args);
        void notOperator(VM* vm, List* args);
        void repl(VM* vm, List* args);
        void tokenize(VM* vm, List* args);
        void type(VM* vm, List* args);
    }

}

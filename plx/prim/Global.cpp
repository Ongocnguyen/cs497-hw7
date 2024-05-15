#include <cassert>

#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/prim/Primitive.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {
    namespace Prim_Global {

        void argMap(VM* vm, List* args) {
            const std::string PRIM_NAME = "argMap";
            checkArgTypes(PRIM_NAME, args, {});
            vm->pushObj(GLOBALS->ArgMap());
        }

        void close(VM* vm, List* args) {
            const std::string PRIM_NAME = "close";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* arg = args->first();
            std::cerr << "prim close called on " << arg << " :: " << arg->typeName() << "\n";
            Triple* env = vm->environment();
            Object* closedArg = arg->close(env);
            vm->pushObj(closedArg);
        }

        void eval(VM* vm, List* args) {
            const std::string PRIM_NAME = "eval";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* arg = args->first();
            vm->pushObj(vm->evalExpr(arg));
        }

        void gc(VM* vm, List* args) {
            const std::string PRIM_NAME = "gc";
            checkArgTypes(PRIM_NAME, args, {});
            vm->setGcNeeded();
            vm->pushObj(GLOBALS->NilObject());
        }

        void notOperator(VM* vm, List* args) {
            const std::string PRIM_NAME = "not";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* arg = args->first();
            vm->pushObj(arg->boolValue() ? GLOBALS->False() : GLOBALS->True());
        }

        void repl(VM* vm, List* args) {
            const std::string PRIM_NAME = "repl";
            checkArgTypes(PRIM_NAME, args, {});
            std::clog << PRIM_NAME << " args = " << args << "\n";
            std::clog << "primitive is not implemented fully\n";
            (void)vm;
            vm->pushObj(GLOBALS->NilObject());
        }

        void tokenize(VM* vm, List* args) {
            const std::string PRIM_NAME = "tokenize";
            checkArgTypes(PRIM_NAME, args, {TypeId::L_STRING});
            String* string = static_cast<String*>(args->first());
            Lexer* lexer = new Lexer();
            List* tokens;
            Object* error;
            if (!lexer->tokenize(string->value(), tokens, error)) {
                // TODO What's supposed to happen here?
                assert(false);
            }
            vm->pushObj(tokens);
        }

        void type(VM* vm, List* args) {
            const std::string PRIM_NAME = "type";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY});
            Object* arg = args->first();
            vm->pushObj(Symbol::create(arg->typeName()));
        }

    }
}

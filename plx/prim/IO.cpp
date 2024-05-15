#include <cassert>

#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {
    namespace Prim_IO {

        void display(VM* vm, List* args) {
            while (!args->isEmpty()) {
                args->first()->displayOn(std::cout);
                args = args->restAsList();
            }
            vm->pushObj(GLOBALS->NilObject());
        }

        void nl(VM* vm, List* args) {
            checkArgTypes("nl", args, {});
            std::cout << std::endl;
            vm->pushObj(GLOBALS->NilObject());
        }

        void readLine(VM* vm, List* args) {
            checkArgTypes("nl", args, {});
            std::string line;
            std::getline(std::cin, line);
            vm->pushObj(new String(line));
        }

        void show(VM* vm, List* args) {
            while (!args->isEmpty()) {
                args->first()->showOn(std::cout);
                args = args->restAsList();
            }
            vm->pushObj(GLOBALS->NilObject());
        }

    }
}

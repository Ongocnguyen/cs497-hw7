#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {

    void argumentCountMismatch(const std::string& name, int nExpectedArgs, int nActualArgs) {
        throwException(name, "Arguemnt count mismatch",
            new Array({new Triple(Symbol::create("Expected"), new Integer(nExpectedArgs)),
                        new Triple(Symbol::create("Actual"), new Integer(nActualArgs))}));
    }

    void checkArgTypes(const std::string& name, List* args, std::initializer_list<TypeId> types) {
        int nExpectedArgs = types.size();
        int nActualArgs = args->length();
        if (nExpectedArgs != nActualArgs) {
            argumentCountMismatch(name, nExpectedArgs, nActualArgs);
        }
        for (TypeId type : types) {
            Object* arg = args->first();
            if (type != TypeId::Z_ANY) {
                if (!arg->isA(type)) {
                    throwException(name, "Arguemnt type mismatch",
                        new Array({new Triple(Symbol::create("Expected"), Symbol::create(TYPE_NAMES[type])),
                                new Triple(Symbol::create("Actual"), Symbol::create(TYPE_NAMES[arg->typeId()]))}));
                }
            }
            args = args->restAsList();
        }
    }

}

#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Method.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/prim/Primitive.hpp>

namespace PLX {

    namespace Prim_Operator {

        void operationNotSupported(std::string primName, Object* lhs, Object* rhs) {
            throwException(primName, "Operation not supported for arguments",
                new Array({
                    new Array({lhs, Symbol::create(lhs->typeName())}),
                    new Array({rhs, Symbol::create(rhs->typeName())}),
                }));
        }

        void bind(VM* vm, List* args) {
            static const std::string PRIM_NAME {"/"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Triple* binding = new Triple(lhs, rhs);
            vm->pushObj(binding);
        }

        void divide(VM* vm, List* args) {
            const std::string PRIM_NAME {"/"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Object* value;
            if (!lhs->divide(rhs, value)) {
                operationNotSupported(PRIM_NAME, lhs, rhs);
            }
            vm->pushObj(value);
        }

        void dot(VM* vm, List* args) {
            const std::string PRIM_NAME {"."};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            if (!rhs->isA(TypeId::E_IDENTIFIER)) {
                std::clog << "Operator::dot lhs = " << lhs << ", rhs = " << rhs << "\n";
                assert(rhs->isA(TypeId::E_IDENTIFIER));
            }
            Identifier* rhsIdent = static_cast<Identifier*>(rhs);
            Method* method = new Method(lhs, rhsIdent);
            vm->pushObj(method);
        }

        void equalTo(VM* vm, List* args) {
            const std::string PRIM_NAME = "==";
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            vm->pushObj((*lhs == *rhs) ? GLOBALS->True() : GLOBALS->False());
        }

        void minus(VM* vm, List* args) {
            const std::string PRIM_NAME {"-"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Object* value;
            if (!lhs->minus(rhs, value)) {
                operationNotSupported(PRIM_NAME, lhs, rhs);
            }
            vm->pushObj(value);
        }

        void plus(VM* vm, List* args) {
            static const std::string PRIM_NAME {"+"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Object* value;
            if (!lhs->plus(rhs, value)) {
                operationNotSupported(PRIM_NAME, lhs, rhs);
            }
            // std::clog << "Operator::plus got value " << value << ", pushing onto ostack\n";
            vm->pushObj(value);
        }

        void percent(VM* vm, List* args) {
            static const std::string PRIM_NAME {"+"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Object* value;
            if (!lhs->percent(rhs, value)) {
                operationNotSupported(PRIM_NAME, lhs, rhs);
            }
            vm->pushObj(value);
        }

        void times(VM* vm, List* args) {
            const std::string PRIM_NAME {"*"};
            checkArgTypes(PRIM_NAME, args, {TypeId::Z_ANY, TypeId::Z_ANY});
            Object* lhs = args->first();
            Object* rhs = args->second();
            Object* value;
            if (!lhs->times(rhs, value)) {
                operationNotSupported(PRIM_NAME, lhs, rhs);
            }
            vm->pushObj(value);
        }

    }
}

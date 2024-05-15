#include <unordered_map>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/InternTable.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    Identifier* Identifier::create(const std::string& name) {
        InternTable* internTable = GLOBALS->IdentifierInternTable();
        Object* identObj;
        if (internTable->lookup(name, identObj)) {
            Identifier* identifier = static_cast<Identifier*>(identObj);
            return identifier;
        }
        Identifier* identifier = new Identifier(name);
        internTable->add(name, identifier);
        return identifier;
    }

    Identifier::Identifier(const std::string& name)
        : Symbolic(name)
    {}

    Object* Identifier::close(Triple* env) {
        std::cerr << "Identifier::close called\n";
        Object* value;
        if (env->lookup(this, value)) {
            return value;
        }
        return this;
    }

    void Identifier::eval(VM* vm) {
        Object* value = vm->lookup(this);
        vm->pushObj(value);
    }

    List* Identifier::freeVars(List* freeVars) {
        return new List(this, freeVars);
    }

    bool Identifier::match(Object* other, Triple*& bindings) {
        bindings = new Triple(this, other, bindings);
        return true;
    }

    TypeId Identifier::typeId() const {
        return TypeId::E_IDENTIFIER;
    }

}

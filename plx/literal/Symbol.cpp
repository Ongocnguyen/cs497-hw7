#include <plx/data/HashTable.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Symbol* Symbol::create(const std::string& name) {
        InternTable* internTable = GLOBALS->SymbolInternTable();
        Object* symbolObj;
        if (internTable->lookup(name, symbolObj)) {
            return static_cast<Symbol*>(symbolObj);
        }
        Symbol* symbol = new Symbol(name);
        internTable->add(name, symbol);
        return symbol;
    }

    Symbol::Symbol(const std::string& name)
        : Symbolic(name)
    {}

    TypeId Symbol::typeId() const {
        return TypeId::L_SYMBOL;
    }

}

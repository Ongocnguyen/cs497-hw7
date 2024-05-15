#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool isEoi(List* tokens) {
        if (tokens->length() != 1) {
            return false;
        }
        Object* tokenObj = tokens->first();
        if (!tokenObj->isA(TypeId::D_ARRAY)) {
            return false;
        }
        Array* token = static_cast<Array*>(tokenObj);
        Object* value;
        if (!token->get(0, value)) {
            return false;
        }
        if (*value != *GLOBALS->EoiSymbol()) {
            return false;
        }
        if (!token->get(1, value)) {
            return false;
        }
        return *value == *GLOBALS->EoiSymbol();
    }

    Array* tokenPos(List* tokens) {
        Object* firstElemObj = tokens->first();
        assert(firstElemObj->isA(TypeId::D_ARRAY));
        Array* firstElem = static_cast<Array*>(firstElemObj);
        assert(firstElem->length() == 3);
        Object* posElemObj;
        assert(firstElem->get(2, posElemObj));
        assert(posElemObj->isA(TypeId::D_ARRAY));
        Array* posElem = static_cast<Array*>(posElemObj);
        return posElem;
    }

}

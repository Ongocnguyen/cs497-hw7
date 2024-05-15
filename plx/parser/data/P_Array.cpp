#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pArray(List*& tokens, Object*& value) {
        if (!pProperListOf(pOpenBrace, pAny, pComma, pCloseBrace, "}", tokens, value)) {
            return false;
        }
        assert(value->isA(TypeId::D_QUEUE));
        Queue* elemQueue = static_cast<Queue*>(value);
        List* elemList = elemQueue->asList();
        Array* elemArray;
        assert(elemList->toArray(elemArray));
        value = elemArray;
        return true;
    }

}

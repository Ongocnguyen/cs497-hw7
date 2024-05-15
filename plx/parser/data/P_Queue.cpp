#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pQueue(List*& tokens, Object*& value) {
        if (!pTilde(tokens, value)) {
            return false;
        }
        if (!pProperListOf(pOpenBracket, pAny, pComma, pCloseBracket, "]", tokens, value)) {
            throwException("Parser", "List of expressions expected after tilde '~'", tokenPos(tokens));
        }
        assert(value->isA(TypeId::D_QUEUE));
        Queue* elemQueue = static_cast<Queue*>(value);
        value = elemQueue;
        return true;
    }

}

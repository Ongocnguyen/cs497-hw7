#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/Do.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_ReservedWords.hpp>

namespace PLX {

    bool pDo(List*& tokens, Object*& value) {
        if (!pReservedDo(tokens, value)) {
            return false;
        }
        pMany(pAny, 0, tokens, value);
        assert(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        pReservedEnd(tokens, value);
        value = new Do(valueQueue->asList());
        return true;
    }

}

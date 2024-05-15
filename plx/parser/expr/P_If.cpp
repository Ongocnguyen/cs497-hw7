#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/If.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_ReservedWords.hpp>
#include <plx/parser/P_Any.hpp>

namespace PLX {

    bool pIf(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pRequireAny,
            pReservedThen,
            pRequireAny,
            pReservedElse,
            pRequireAny
        };
        if (!pReservedIf(tokens, value)) {
            return false;
        }
        pSeq(parserList, tokens, value);
        assert(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        Object* condition = valueQueue->deq();
        Object* consequent = valueQueue->deq();
        Object* alternate = valueQueue->deq();
        value = new If(condition, consequent, alternate);
        return true;
    }

}
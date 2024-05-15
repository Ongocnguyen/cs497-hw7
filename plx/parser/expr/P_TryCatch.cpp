#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/TryCatch.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_ReservedWords.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    Triple* bindingQueueToTriple(Queue* queue);
    bool pBinding(List*& tokens, Object*& value);
    bool pBar(List*& tokens, Object*& value);
    bool pRequireReservedIn(List*& tokens, Object*& value);
    bool pRequireAny(List*& tokens, Object*& value);
    bool pRules(List*& tokens, Object*& value);

    bool pRequireReservedCatch(List*& tokens, Object*& value) {
        if (!pReservedCatch(tokens, value)) {
            throwException("Parser", "Keyword 'catch' expected", tokenPos(tokens));
        }
        return true;
    }

    bool pMaybeFinally(List*& tokens, Object*& value) {
        if (!pReservedFinally(tokens, value)) {
            value = GLOBALS->NilObject();
            return true;
        }
        return pRequireAny(tokens, value);
    }

    bool pTryCatch(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pReservedTry,
            pRequireAny,
            pRequireReservedCatch,
            pRules,
            pMaybeFinally,
            pReservedEnd
        };
        Object* partsObj;
        if (!pSeq(parserList, tokens, partsObj)) {
            return false;
        }
        assert(partsObj->isA(TypeId::D_QUEUE));
        Queue* partsQueue = static_cast<Queue*>(partsObj);
        Object* expr = partsQueue->deq();
        Object* rulesObj = partsQueue->deq();
        assert(rulesObj->isA(TypeId::D_QUEUE));
        Queue* rulesQueue = static_cast<Queue*>(rulesObj);
        Triple* rules = bindingQueueToTriple(rulesQueue);
        Object* finally = partsQueue->deq();
        TryCatch* tryCatch = new TryCatch(expr, rules, finally);
        value = tryCatch;
        return true;
    }

}

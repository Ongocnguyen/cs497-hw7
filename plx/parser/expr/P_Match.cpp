#include <cassert>

#include <plx/expr/Match.hpp>
#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Any.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_ReservedWords.hpp>
#include <plx/parser/P_SpecialChars.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pBinding(List*& tokens, Object*& value);

    bool pRules(List*& tokens, Object*& value) {
        if (!pSepBy(pBinding, pBar, 1, tokens, value)) {
            throwException("Parser", "Rules expected", tokenPos(tokens));
        }
        return true;
    }

    Triple* bindingQueueToTriple(Queue* queue) {
        Object* firstRuleObj = queue->deq();
        assert(firstRuleObj->isA(TypeId::D_TRIPLE));
        Triple* firstRule = static_cast<Triple*>(firstRuleObj);
        Triple* rule = firstRule;
        while (!queue->isEmpty()) {
            Object* nextRuleObj = queue->deq();
            assert(nextRuleObj->isA(TypeId::D_TRIPLE));
            Triple* nextRule = static_cast<Triple*>(nextRuleObj);
            rule->setNext(nextRule);
            rule = nextRule;
        }
        return firstRule;
    }

    bool pMatch(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pReservedMatch,
            pRequireAny,
            pRequireReservedIn,
            pRules,
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
        Match* match = new Match(expr, rules);
        value = match;
        return true;
    }

}

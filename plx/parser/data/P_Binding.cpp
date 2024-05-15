#include <cassert>
#include <list>

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/parser/data/P_Data.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/literal/P_Literal.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/P_Any.hpp>

namespace PLX {

    bool pBindingLhs(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pLiteral, pDataWithoutBinding, pIdentifier
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pBinding(List*& tokens, Object*& value)  {
        List* savedTokens = tokens;
        static const std::initializer_list<Parser> parserList {
            pBindingLhs, pIgnoreEquals, pRequireAny
        };
        if (!pSeq(parserList, tokens, value)) {
            tokens = savedTokens;
            return false;
        }
        assert(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        Object* lhs = valueQueue->deq();
        Object* rhs= valueQueue->deq();
        value = new Triple(lhs, rhs);
        return true;
    }

    bool pRequireBindingList(int minCount, List*& tokens, Object*& value) {
        if (!pBindingList(minCount, tokens, value)) {
            throwException("Parser", "List of bindings expected", tokenPos(tokens));
        }
        return true;
    }

    bool pBindingList(int minCount, List*& tokens, Object*& value) {
        if (!pSepBy(pBinding, pComma, minCount, tokens, value)) {
            return false;
        }
        assert(value->isA(TypeId::D_QUEUE));
        Queue* valueQueue = static_cast<Queue*>(value);
        Triple* first = GLOBALS->EmptyTriple();;
        Triple* last;
        int nIters = 0;
        while (!valueQueue->isEmpty()) {
            Object* tripleObj = valueQueue->deq();
            assert(tripleObj->isA(TypeId::D_TRIPLE));
            Triple* triple = static_cast<Triple*>(tripleObj);
            if (nIters == 0) {
                first = triple;
                last = first;
            }
            else {
                last->setNext(triple);
                last = triple;
            }
            nIters++;
        }
        if (nIters < minCount) {
            return false;
        }
        value = first;
        return true;
    }

}
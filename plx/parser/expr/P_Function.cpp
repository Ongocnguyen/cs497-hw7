#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/NamedFunction.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pLiteral(List*& tokens, Object*& value);
    bool pData(List*& tokens, Object*& value);

    bool pReservedFun(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "fun", tokens, value);
    }

    bool pParameter(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pIdentifier, pLiteral, pData
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pParameterList(List*& tokens, Object*& value) {
        return pProperListOf(pOpenParen, pParameter, pComma, pCloseParen, ")", tokens, value);
    }

    bool pRule(List*& tokens, Object*& value) {
        Object* paramObj;
        if (!pParameterList(tokens, paramObj)) {
            throwException("Parser", "Parameter list expected", tokenPos(tokens));
        }
        Object* ignore;
        if (!pEquals(tokens, ignore)) {
            throwException("Parser", "Equal sign expected", tokenPos(tokens));
        }
        Object* body;
        if (!pAny(tokens, body)) {
            throwException("Parser", "Function body expected", tokenPos(tokens));
        }
        assert(paramObj->isA(TypeId::D_QUEUE));
        Queue* paramQueue = static_cast<Queue*>(paramObj);
        List* paramList = paramQueue->asList();
        Function* rule = new Function(paramList, body);
        value = rule;
        return true;
    }

    bool pFunction(List*& tokens, Object*& value) {
        Object* ignore;
        if (!pReservedFun(tokens, ignore)) {
            return false;
        }
        Object* nameObj = GLOBALS->NilObject();
        pIdentifier(tokens, nameObj);
        Object* rulesObj;
        pSepBy(pRule, pBar, 1, tokens, rulesObj);
        assert(rulesObj->isA(TypeId::D_QUEUE));
        Queue* rulesQueue = static_cast<Queue*>(rulesObj);
        // construct the function from the rules queue
        Object* functionObj = rulesQueue->deq();
        assert(functionObj->isA(TypeId::E_FUNCTION));
        Function* rule = static_cast<Function*>(functionObj);
        Function* function = rule;
        while (!rulesQueue->isEmpty()) {
            Object* functionObj = rulesQueue->deq();
            assert(functionObj->isA(TypeId::E_FUNCTION));
            Function* nextRule = static_cast<Function*>(functionObj);
            rule->setNext(nextRule);
            rule = nextRule;
        }
        // determine if the function is named or anonymous
        if (nameObj->isA(TypeId::L_NIL)) {
            value = function;
        }
        else {
            assert(nameObj->isA(TypeId::E_IDENTIFIER));
            Identifier* name = static_cast<Identifier*>(nameObj);
            value = new NamedFunction(name, function);
        }
        return true;
    }

}

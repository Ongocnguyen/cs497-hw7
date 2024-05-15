#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pArgumentList(List*& tokens, Object*& value) {
        return pProperListOf(pOpenParen, pAny, pComma, pCloseParen, ")", tokens, value);
    }

    bool pApply(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pIdentifier,
            pParenExpr
        };
        Object* abstrObj;
        if (!pOneOf(parserList, tokens, abstrObj)) {
            return false;
        }
        Object* argumentListObj;
        if (!pArgumentList(tokens, argumentListObj)) {
            return false;
        }
        assert(argumentListObj->isA(TypeId::D_QUEUE));
        Queue* argumentListQueue = static_cast<Queue*>(argumentListObj);
        List* argumentList = argumentListQueue->asList();
        Apply* apply = new Apply(abstrObj, argumentList);
        value = apply;
        return true;
    }

}

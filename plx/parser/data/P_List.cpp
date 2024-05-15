#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pRequireAny(List*& tokens, Object*& value);

    bool pImproperElement(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pBar,
            pRequireAny,
            pCloseBracket
        };
        return pSeq(parserList, tokens, value);
    }

    bool pTerminal(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pImproperElement,
            pCloseBracket
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pList(List*& tokens, Object*& value) {
        if (!pImproperListOf(pOpenBracket, pAny, pComma, pTerminal, "]", tokens, value)) {
            return false;
        }
        return true;
    }

}

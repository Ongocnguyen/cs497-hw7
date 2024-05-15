#include <plx/data/List.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/P_Primitive.hpp>

namespace PLX {

    bool pBoolean(List*& tokens, Object*& value) {
        return pSpotAndStrip(GLOBALS->BooleanSymbol(), tokens, value);
    }

    bool pInteger(List*& tokens, Object*& value) {
        return pSpotAndStrip(GLOBALS->IntegerSymbol(), tokens, value);
    }

    bool pNil(List*& tokens, Object*& value) {
        return pSpotAndStrip(GLOBALS->NilSymbol(), tokens, value);
    }

    bool pString(List*& tokens, Object*& value) {
        return pSpotAndStrip(GLOBALS->StringSymbol(), tokens, value);
    }

    bool pSymbol(List*& tokens, Object*& value) {
        return pSpotAndStrip(GLOBALS->SymbolSymbol(), tokens, value);
    }

    bool pLiteral(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pBoolean, pInteger, pNil, pString, pSymbol
        };
        return pOneOf(parserList, tokens, value);
    }

}

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pOpenBrace(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "{", tokens, value);
    }

    bool pCloseBrace(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "}", tokens, value);
    }

    bool pOpenBracket(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "[", tokens, value);
    }

    bool pCloseBracket(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "]", tokens, value);
    }

    bool pOpenParen(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "(", tokens, value);
    }

    bool pCloseParen(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), ")", tokens, value);
    }

    bool pComma(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), ",", tokens, value);
    }

    bool pTilde(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "~", tokens, value);
    }

    bool pEquals(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->IdentifierSymbol(), Identifier::create("="), tokens, value);
    }

    bool pHashMark(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "#", tokens, value);
    }

    bool pIgnoreEquals(List*& tokens, Object*& value) {
        if (!pEquals(tokens, value)) {
            return false;
        }
        value = GLOBALS->ParseIgnoreSymbol();
        return true;
    }

    bool pRequireCloseParen(List*& tokens, Object*& value) {
        if (!pCloseParen(tokens, value)) {
            throwException("Parser", "Closing parenthesis ')' expected", tokenPos(tokens));
        }
        return true;
    }

    bool pQuoteChar(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "'", tokens, value);
    }

    bool pRequireQuoteChar(List*& tokens, Object*& value) {
        if (!pQuoteChar(tokens, value)) {
            throwException("Parser", "Closing quote character expected", tokenPos(tokens));
        }
        return true;
    }

    bool pBar(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->SpecialSymbol(), "|", tokens, value);
    }

}

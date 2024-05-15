#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Primitive.hpp>

namespace PLX {

    // Reserved words that begin an expression are not "required".
    // A parse of one of these reserved words can simply fail
    // and not throw an exception.

    bool pReservedDo(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "do", tokens, value);
    }

    bool pReservedIf(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "if", tokens, value);
    }

    bool pReservedFinally(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "finally", tokens, value);
    }

    bool pReservedLet(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "let", tokens, value);
    }

    bool pReservedMatch(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "match", tokens, value);
    }

    bool pReservedTry(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "try", tokens, value);
    }

    // Reserved words inside an expression are required.
    // A parse failure of one of these words should actually throw
    // an exception.

    bool pRequireReservedWord(const std::string& reservedWord, List*& tokens, Object*& value) {
        if (!pSpotValue(GLOBALS->ReservedSymbol(), reservedWord, tokens, value)) {
            std::string message = "Reserved word'" + reservedWord + "' expected";
            throwException("Parser", message, tokenPos(tokens));
        }
        value = GLOBALS->ParseIgnoreSymbol();
        return true;
    }

    bool pReservedCatch(List*& tokens, Object*& value) {
        return pRequireReservedWord("catch", tokens, value);
    }

    bool pReservedElse(List*& tokens, Object*& value) {
        return pRequireReservedWord("else", tokens, value);
    }

    bool pReservedEnd(List*& tokens, Object*& value) {
        return pRequireReservedWord("end", tokens, value);
    }

    bool pReservedIn(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "in", tokens, value);
    }

    bool pRequireReservedIn(List*& tokens, Object*& value) {
        return pRequireReservedWord("in", tokens, value);
    }

    bool pReservedThen(List*& tokens, Object*& value) {
        return pRequireReservedWord("then", tokens, value);
    }

}

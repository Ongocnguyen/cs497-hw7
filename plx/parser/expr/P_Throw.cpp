#include <cassert>

#include <plx/expr/Throw.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pRequireAny(List*& tokens, Object*& value);

    bool pReservedThrow(List*& tokens, Object*& value) {
        return pSpotValue(GLOBALS->ReservedSymbol(), "throw", tokens, value);
    }

    bool pThrow(List*& tokens, Object*& value) {
        if (!pReservedThrow(tokens, value)) {
            return false;
        }
        Object* throwValue;
        pRequireAny(tokens, throwValue);
        Throw* throwExpr = new Throw(throwValue);
        value = throwExpr;
        return true;
    }

}

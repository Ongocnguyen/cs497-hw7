#pragma once

#include <plx/data/List.hpp>
#include <plx/object/Globals.hpp>
#include <plx/parser/P_Primitive.hpp>

namespace PLX {

    bool pBoolean(List*& tokens, Object*& value);
    bool pInteger(List*& tokens, Object*& value);
    bool pNil(List*& tokens, Object*& value);
    bool pString(List*& tokens, Object*& value);
    bool pSymbol(List*& tokens, Object*& value);
    bool pLiteral(List*& tokens, Object*& value);

}

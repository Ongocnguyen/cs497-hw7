#pragma once

#include <plx/data/List.hpp>

namespace PLX {

    typedef bool (*Parser)(List*& tokens, Object*& value);

    bool isEoi(List* tokens);
    Array* tokenPos(List* tokens);

    bool pAny(List*& tokens, Object*& value);
    Array* tokenPos(List* tokens);

}

#pragma once

#include <map>
#include <string>

namespace PLX {

    enum class TypeId {
        D_ARRAY,
        D_CLOSURE,
        D_HASHTABLE,
        D_INPUTSTREAM,
        D_INTERNTABLE,
        D_METHOD,
        D_LIST,
        D_QUEUE,
        D_TRIPLE,
        E_APPLY,
        E_BINOPEXPR,
        E_FUNCTION,
        E_IDENTIFIER,
        E_INDEXOP,
        E_IF,
        E_LET,
        E_LETIN,
        E_MATCH,
        E_NAMEDFUNCTION,
        E_PARENEXPR,
        E_QUOTE,
        E_SEQ,
        E_THROW,
        E_TRYCATCH,
        L_BOOLEAN,
        L_INTEGER,
        L_NIL,
        L_PRIMITIVEFUNCTION,
        L_STRING,
        L_SYMBOL,
        C_APPLY,
        C_CATCH,
        C_DROP,
        C_FINALLY,
        C_METHOD,
        C_RESTOREENV,
        C_THROWCONTIN,
        C_CONTINUATION,
        T_LEXER,
        T_OBJECT,
        T_PARSER,
        T_READEVALPRINT,
        T_REPL,
        T_VM,
        // The Z_ANY entry serves two purposes:
        // It's the array-size sentinel for HashCode,
        // and it's a way to type-check for any kind of value.
        Z_ANY
    };

    extern std::map<TypeId, std::string> TYPE_NAMES;

}

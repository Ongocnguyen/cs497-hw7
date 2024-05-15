#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp> 
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/parser/data/P_Data.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/literal/P_Literal.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pData(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pArray,
            pBinding,
            pList,
            pQueue,
            pHashTable
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pDataWithoutBinding(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pArray,
            pList,
            pQueue,
            pHashTable
        };
        return pOneOf(parserList, tokens, value);
    }

}

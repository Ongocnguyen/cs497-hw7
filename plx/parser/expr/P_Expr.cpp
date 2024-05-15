#include <cassert>

#include <plx/data/List.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pExpr(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            // pBinOpExpr,
            pApply,
            pDo,
            pFunction,
            pIdentifier,
            pIf,
            pLet,
            pMatch,
            pParenExpr,
            pQuote,
            pThrow,
            pTryCatch
        };
        return pOneOf(parserList, tokens, value);
    }

}

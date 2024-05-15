#include <plx/expr/ParenExpr.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_SpecialChars.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pRequireAny(List*& tokens, Object*& value);

    bool pParenExpr(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pOpenParen,
            pRequireAny,
            pRequireCloseParen
        };
        Object* expr;
        if (!pSeq(parserList, tokens, expr)) {
            return false;
        }
        value = new ParenExpr(expr);
        return true;
    }

}

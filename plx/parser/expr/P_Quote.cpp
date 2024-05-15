#include <plx/data/List.hpp>
#include <plx/expr/Quote.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pRequireAny(List*& tokens, Object*& value);

    bool pQuote(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pQuoteChar, pRequireAny, pRequireQuoteChar
        };
        if (!pSeq(parserList, tokens, value)) {
            return false;
        }
        value = new Quote(value);
        return true;
    }

}

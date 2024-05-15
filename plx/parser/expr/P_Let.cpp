#include <cassert>

#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Let.hpp>
#include <plx/expr/LetIn.hpp>
#include <plx/object/Object.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_ReservedWords.hpp>

namespace PLX {

    bool pRequireAny(List*& tokens, Object*& value);
    bool pRequireBindingList(int minCount, List*& tokens, Object*& value);

    bool pLetBindings(List*& tokens, Object*& value) {
        return pRequireBindingList(1, tokens, value);
    }

    bool pLet(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pReservedLet, pLetBindings
        };
        List* savedTokens = tokens;
        Object* parts;
        if (!pSeq(parserList, tokens, parts)) {
            tokens = savedTokens;
            return false;
        }
        assert(parts->isA(TypeId::D_TRIPLE));
        Triple* bindings = static_cast<Triple*>(parts);
        Object* ignore;
        if (!pReservedIn(tokens, ignore)) {
           value = new Let(bindings);
        }
        else {
            Object* body;
            pRequireAny(tokens, body);
            value = new LetIn(bindings, body);
        }
        return true;
    }

}
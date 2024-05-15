#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>

namespace PLX {

    bool pBindingList(int minCount, List*& tokens, Object*& value);

    bool pHashTable(List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        if (!pSeq({pHashMark, pOpenBrace}, tokens, value)) {
            tokens = savedTokens;
            return false;
        }
        pBindingList(0, tokens, value);
        assert(value->isA(TypeId::D_TRIPLE));
        Triple* bindings = static_cast<Triple*>(value);
        if (!pCloseBrace(tokens, value)) {
            throwException("Parser", "Binding or closing brace expected", tokenPos(tokens));
        }
        HashTable* hashTable = new HashTable();
        while (!bindings->isEmpty()) {
            hashTable->put(bindings->key(), bindings->value());
            bindings = bindings->next();
        }
        value = hashTable;
        return true;
    }

}

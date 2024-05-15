#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp> 
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>

namespace PLX {

    bool pBar(List*& tokens, Object*& value);
    bool pRequireAny(List*& tokens, Object*& value);

    Object* strip(Object* tokenObj) {
        assert(tokenObj->isA(TypeId::D_ARRAY));
        Array* tokenArray = static_cast<Array*>(tokenObj);
        assert(tokenArray->length() > 1);
        Object* elem;
        assert(tokenArray->get(1, elem));
        return elem;
    }

    bool pSpot(Symbol* tokenType, List*& tokens, Object*& value) {
        if (tokens->isEmpty()) {
            return false;
        }
        // The assert() statements here ensure that the token is properly formed.
        assert(tokens->first()->isA(TypeId::D_ARRAY));
        Array* firstToken = static_cast<Array*>(tokens->first());
        assert(firstToken->length() > 0);
        Object* elem;
        assert(firstToken->get(0, elem));
        if (elem->equals(tokenType)) {
            value = firstToken;
            tokens = tokens->restAsList();
            return true;
        }
        return false;
    }

    bool pProperListOf(Parser pOpen, Parser pElement, Parser pSeparator, Parser pClose,
            const std::string& closingCharacter,
            List*& tokens, Object*& value) {
        if (!pOpen(tokens, value)) {
            return false;
        }
        List* savedTokens = tokens;
        if (!pSepBy(pElement, pSeparator, 0, tokens, value)) {
            tokens = savedTokens;
            return false;
        }
        Object* savedValue = value;
        if (!pClose(tokens, value)) {
            throwException("Parser", "Closing character '" + closingCharacter + "' expected", tokenPos(tokens));
        }
        value = savedValue;
        return true;
    }

    bool pImproperListOf(Parser pOpen, Parser pElement, Parser pSeparator, Parser pClose,
            const std::string& closingCharacter,
            List*& tokens, Object*& value) {
        if (!pOpen(tokens, value)) {
            return false;
        }
        List* savedTokens = tokens;
        Object* queueObj;
        if (!pSepBy(pElement, pSeparator, 0, tokens, queueObj)) {
            tokens = savedTokens;
            return false;
        }
        bool isImproper = false;
        Object* ignore;
        Object* finalValue;
        if (pBar(tokens, ignore)) {
            pRequireAny(tokens, finalValue);
            isImproper = true;
        }
        if (!pClose(tokens, ignore)) {
            throwException("Parser", "Closing character '" + closingCharacter + "' expected", tokenPos(tokens));
        }
        assert(queueObj->isA(TypeId::D_QUEUE));
        Queue* queue = static_cast<Queue*>(queueObj);
        List* list = queue->asList();
        if (isImproper) {
            // locate the final pair in the list
            List* head = queue->asList();
            while (!head->restAsList()->isEmpty()) {
                head = head->restAsList();
            }
            head->setRest(finalValue);
        }
        value = list;
        return true;
    }

    bool pMany(Parser parser, int minCount, List*& tokens, Object*& value) {
        Queue* valueQueue = new Queue();
        int count = 0;
        while (parser(tokens, value)) {
            valueQueue->enq(value);
            count++;
        }
        if (count < minCount) {
            return false;
        }
        value = valueQueue;
        return true;
    }

    bool pOneOf(std::initializer_list<Parser> parsers, List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        for (Parser parser : parsers) {
            if (parser(tokens, value)) {
                return true;
            }
            tokens = savedTokens;
        }
        return false;
    }

    bool pSepBy(Parser pElem, Parser pSeparator, int minCount, List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        Queue* elems = new Queue();
        int count = 0;
        while (true) {
            if (count > 0) {
                if (!pSeparator(tokens, value)) {
                    break;
                }
            }
            if (!pElem(tokens, value)) {
                if (count > 0) {
                    throwException("Parser", "Element expected after separator", tokenPos(tokens));
                }
                break;
            }
            elems->enq(value);
            count++;
        }
        if (count < minCount) {
            tokens = savedTokens;
            return false;
        }
        value = elems;
        return true;
    }

    bool pSeq(std::initializer_list<Parser> parsers, List*& tokens, Object*& value) {
        Queue* results = new Queue();
        for (Parser parser : parsers) {
            if (!parser(tokens, value)) {
                return false;
            }
            if (value != GLOBALS->ParseIgnoreSymbol()) {
                results->enq(value);
            }
        }
        if (results->length() == 1) {
            value = results->deq();
        }
        else {
            value = results;
        }
        return true;
    }


    bool pSpotAndStrip(Symbol* tokenType, List*& tokens, Object*& value) {
        if (!pSpot(tokenType, tokens, value)) {
            return false;
        }
        value = strip(value);
        return true;
    }

    bool pSpotValue(Symbol* tokenType, const std::string& tokenValue, List*& tokens, Object*& value) {
        return pSpotValue(tokenType, new String(tokenValue), tokens, value);
    }

    bool pSpotValue(Symbol* tokenType, Object* tokenValue, List*& tokens, Object*& value) {
        if (!pSpotValue_noIgnore(tokenType, tokenValue, tokens, value)) {
            return false;
        }
        value = GLOBALS->ParseIgnoreSymbol();
        return true;
    }

    bool pSpotValue_noIgnore(Symbol* tokenType, Object* tokenValue, List*& tokens, Object*& value) {
        if (tokens->isEmpty()) {
            return false;
        }
        // The assert() statements here ensure that the token is properly formed.
        assert(tokens->first()->isA(TypeId::D_ARRAY));
        Array* firstToken = static_cast<Array*>(tokens->first());
        assert(firstToken->length() > 1);
        Object* elem;
        assert(firstToken->get(0, elem));
        if (elem->equals(tokenType)) {
            assert(firstToken->get(1, value));
            if (tokenValue->equals(value)) {
                tokens = tokens->restAsList();
                return true;
            }
        }
        return false;
    }

}

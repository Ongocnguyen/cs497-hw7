#pragma once

#include <list>

#include <plx/parser/Parser.hpp>

namespace PLX {

    class Object;
    class List;
    class Symbol;

    Object* strip(Object* tokenObj);

    bool pOpenBrace(List*& tokens, Object*& value);
    bool pCloseBrace(List*& tokens, Object*& value);
    bool pOpenBracket(List*& tokens, Object*& value);
    bool pCloseBracket(List*& tokens, Object*& value);
    bool pOpenParen(List*& tokens, Object*& value);
    bool pCloseParen(List*& tokens, Object*& value);
    bool pComma(List*& tokens, Object*& value);
    bool pTilde(List*& tokens, Object*& value);
    bool pEquals(List*& tokens, Object*& value);
    bool pHashMark(List*& tokens, Object*& value);

    bool pProperListOf(Parser pOpen, Parser pElement, Parser pSeparator, Parser pClose,
            const std::string& closeCharacter,
            List*& tokens, Object*& value);
    bool pImproperListOf(Parser pOpen, Parser pElement, Parser pSeparator, Parser pClose,
            const std::string& closeCharacter,
            List*& tokens, Object*& value);
    bool pMany(Parser parser, int minCount, List*& tokens, Object*& value);
    bool pOneOf(std::initializer_list<Parser> parsers, List*& tokens, Object*& value);
    bool pSepBy(Parser elem, Parser separator, int minCount, List*& tokens, Object*& value);
    bool pSeq(std::initializer_list<Parser> parsers, List*& tokens, Object*& value);
    bool pSpot(Symbol* tokenType, List*& tokens, Object*& value);
    bool pSpotAndStrip(Symbol* tokenType, List*& tokens, Object*& value);
    bool pSpotValue(Symbol* tokenType, const std::string& tokenValue, List*& tokens, Object*& value);
    bool pSpotValue(Symbol* tokenType, Object* tokenValue, List*& tokens, Object*& value);
    bool pSpotValue_noIgnore(Symbol* tokenType, Object* tokenValue, List*& tokens, Object*& value);

}

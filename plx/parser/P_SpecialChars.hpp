namespace PLX {

    class List;
    class Object;

    bool pBar(List*& tokens, Object*& value);
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
    bool pIgnoreEquals(List*& tokens, Object*& value);
    bool pRequireCloseParen(List*& tokens, Object*& value);
    bool pQuoteChar(List*& tokens, Object*& value);
    bool pRequireQuoteChar(List*& tokens, Object*& value);

}

namespace PLX {

    class List;
    class Object;

    bool pReservedCatch(List*& tokens, Object*& value);
    bool pReservedDo(List*& tokens, Object*& value);
    bool pReservedElse(List*& tokens, Object*& value);
    bool pReservedEnd(List*& tokens, Object*& value);
    bool pReservedFinally(List*& tokens, Object*& value);
    bool pReservedFun(List*& tokens, Object*& value);
    bool pReservedIf(List*& tokens, Object*& value);
    bool pReservedIn(List*& tokens, Object*& value);
    bool pRequireReservedIn(List*& tokens, Object*& value);
    bool pReservedLet(List*& tokens, Object*& value);
    bool pReservedMatch(List*& tokens, Object*& value);
    bool pReservedThen(List*& tokens, Object*& value);
    bool pReservedThrow(List*& tokens, Object*& value);
    bool pReservedTry(List*& tokens, Object*& value);

}
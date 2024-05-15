#pragma once

namespace PLX {

    class List;
    class Object;

    bool pApply(List*& tokens, Object*& value);
    bool pBinOpExpr(List*& tokens, Object*& value);
    bool pDo(List*& tokens, Object*& value);
    bool pFunction(List*& tokens, Object*& value);
    bool pIdentifier(List*& tokens, Object*& value);
    bool pIf(List*& tokens, Object*& value);
    bool pLet(List*& tokens, Object*& value);
    bool pLetIn(List*& tokens, Object*& value);
    bool pMatch(List*& tokens, Object*& value);
    bool pParenExpr(List*& tokens, Object*& value);
    bool pQuote(List*& tokens, Object*& value);
    bool pThrow(List*& tokens, Object*& value);
    bool pTryCatch(List*& tokens, Object*& value);
    bool pExpr(List*& tokens, Object*& value);

}

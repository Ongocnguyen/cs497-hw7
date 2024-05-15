#include <cassert>
#include <list>

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/Apply.hpp>
#include <plx/expr/BinOpExpr.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/IndexOp.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/P_SpecialChars.hpp>
#include <plx/parser/expr/P_Expr.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/object/ThrowException.hpp>

namespace PLX {

    bool pLiteral(List*& tokens, Object*& value);
    bool pData(List*& tokens, Object*& value);
    bool pExpr(List*& tokens, Object*& value);
    bool pIdentifier(List*& tokens, Object*& value);
    bool pSpotOperator(List*& tokens, Object*& value);
    bool pArgumentList(List*& tokens, Object*& value);
    bool pDataWithoutBinding(List*& tokens, Object*& value);

    bool pLevel1(List*& tokens, Object*& value);
    bool pLevel2(List*& tokens, Object*& value);
    bool pLevel3(List*& tokens, Object*& value);
    bool pLevel4(List*& tokens, Object*& value);
    bool pLevel5(List*& tokens, Object*& value);

    bool pRequireAny(List*& tokens, Object*& value) {
        if (!pAny(tokens, value)) {
            throwException("Parser", "Any expression expected", tokenPos(tokens));
        }
        return true;
    }

    bool pSpotOperator(Identifier* oper, List*& tokens, Object*& value) {
        if (pSpotValue(GLOBALS->IdentifierSymbol(), oper, tokens, value)) {
            value = oper;
            return true;
        }
        return false;
    }

    bool pEqualOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("=")};
        bool res = pSpotOperator(oper, tokens, value);
        return res;
    }

    bool pEqualToOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("==")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pPlusOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("+")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pMinusOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("-")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pMultiplyOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("*")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pDivideOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("/")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pModOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create("%")};
        return pSpotOperator(oper, tokens, value);
    }
    
    bool pDotOp(List*& tokens, Object*& value) {
        static Identifier* oper {Identifier::create(".")};
        return pSpotOperator(oper, tokens, value);
    }

    bool pLevel1op(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pEqualOp, pEqualToOp
        };
        bool res = pOneOf(parserList, tokens, value);
        return res;
    }

    bool pLevel2op(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pPlusOp, pMinusOp
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pLevel3op(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pMultiplyOp, pDivideOp, pModOp
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pLevel4op(List*& tokens, Object*& value) {
    #if 0
        static const std::initializer_list<Parser> parserList {
            pDot
        };
        return pOneOf(parserList, tokens, value);
    #else
        // so far there's only one parser a this level, so pOneOf is not really needed
        return pDotOp(tokens, value);
    #endif
    }

    bool pIndexer(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pOpenBracket,
            pAny,
            pCloseBracket
        };
        return pSeq(parserList, tokens, value);
    }

    void operatorException(Identifier* oper, List* tokens) {
        std::string message {"Expression expected after '" + oper->toString() + "' operator"};
        throwException("Parser", message, tokenPos(tokens));
    }

    bool pAny(List*& tokens, Object*& value) {
        Object* lhs;
        if (!pLevel1(tokens, lhs)) {
            return false;
        }
        Object* operObj;
        if (pLevel1op(tokens, operObj)) {
            assert(operObj->isA(TypeId::E_IDENTIFIER));
            Identifier* operIdent = static_cast<Identifier*>(operObj);
            Object* rhs;
            if (!pAny(tokens, rhs)) {
                // throwException("Parser", "Expression expected after operator", tokenPos(tokens));
                operatorException(operIdent, tokens);
            }
            value = new BinOpExpr(lhs, operIdent, rhs);
            return true;
        }
        value = lhs;
        return true;
    }

    bool pLevel1(List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        Object* lhs;
        if (!pLevel2(tokens, lhs)) {
            tokens = savedTokens;
            return false;
        }
        savedTokens = tokens;
        Object* operObj;
        if (pLevel2op(tokens, operObj)) {
            assert(operObj->isA(TypeId::E_IDENTIFIER));
            Identifier* operIdent = static_cast<Identifier*>(operObj);
            Object* rhs;
            if (!pAny(tokens, rhs)) {
                // throwException("Parser", "Expression expected after operator", tokenPos(tokens));
                operatorException(operIdent, tokens);
            }
            value = new BinOpExpr(lhs, operIdent, rhs);
            return true;
        }
        tokens = savedTokens;
        value = lhs;
        return true;
    }

    bool pLevel2(List*& tokens, Object*& value) {
        Object* lhs;
        if (!pLevel3(tokens, lhs)) {
            return false;
        }
        Object* operObj;
        if (pLevel3op(tokens, operObj)) {
            assert(operObj->isA(TypeId::E_IDENTIFIER));
            Identifier* operIdent = static_cast<Identifier*>(operObj);
            Object* rhs;
            if (!pAny(tokens, rhs)) {
                // throwException("Parser", "Expression expected after operator", tokenPos(tokens));
                operatorException(operIdent, tokens);
            }
            value = new BinOpExpr(lhs, operIdent, rhs);
            return true;
        }
        value = lhs;
        return true;
    }

    bool pLevel3(List*& tokens, Object*& value) {
        Object* lhs;
        if (!pLevel4(tokens, lhs)) {
            return false;
        }
        while (true) {
            Object* argsObj;
            if (pArgumentList(tokens, argsObj)) {
                assert(argsObj->isA(TypeId::D_QUEUE));
                Queue* argsQueue = static_cast<Queue*>(argsObj);
                List* argumentList = argsQueue->asList();
                lhs = new Apply(lhs, argumentList);
                continue;
            }
            Object* index;
            if (pIndexer(tokens, index)) {
                lhs = new IndexOp(lhs, index);
                continue;
            }
            break;
        }
        value = lhs;
        return true;
    }

    bool pDotRhs(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pIdentifier,
            pParenExpr
        };
        return pOneOf(parserList, tokens, value);
    }

    bool pLevel4(List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        Object* lhs;
        if (!pLevel5(tokens, lhs)) {
            tokens = savedTokens;
            return false;
        }
        while (true) {
            Object* operObj;
            if (pLevel4op(tokens, operObj)) {
                Identifier* operIdent = static_cast<Identifier*>(operObj);
                Object* rhs;
                if (!pDotRhs(tokens, rhs)) {
                    operatorException(operIdent, tokens);
                }
                lhs = new BinOpExpr(lhs, operIdent, rhs);
            }
            else {
                break;
            }
        }
        value = lhs;
        return true;
    }

    bool pLevel5(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pExpr,
            pDataWithoutBinding,
            pLiteral
        };
        return pOneOf(parserList, tokens, value);
    }

}

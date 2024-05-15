#include <cassert>

#include <plx/data/Array.hpp>
#include <plx/data/Queue.hpp>
#include <plx/expr/BinOpExpr.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/parser/literal/P_Literal.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/parser/Parser.hpp>
#include <plx/parser/data/P_Data.hpp>

namespace PLX {

/*
    ops1 = [':=', '~', '==', '!=', '<', '>', '<=', '>=', '##', '=', ':?']
    ops2 = ['+', '-', '::', ':>', '++', '+++', '#+', '#-']
    ops3 = ['*', '/', '%', '#*', '#/']
    ops4 = ['^']
    ops5 = ['.', ':']

    self.add('binop', MakeBinOpParser(SepBy(1, 'lvl2', OneOf(*ops1), 'exprErr', True)))
    self.add('lvl2', MakeBinOpParser(SepBy(1, 'lvl3', OneOf(*ops2), 'exprErr', True)))
    self.add('lvl3', MakeBinOpParser(SepBy(1, 'lvl4', OneOf(*ops3), 'exprErr', True)))
    self.add('lvl4', MakeBinOpParser(SepBy(1, 'lvl5', OneOf(*ops4), 'exprErr', True)))
    self.add('lvl5', MakeBinOpParser(SepBy(1, 'lvl6', OneOf(*ops5), 'exprErr', True)))
    self.add('lvl6', OneOf('construct', 'container', 'literal'))
*/

    bool pSpotOperator(List*& tokens, Object*& value) {
        Object* operatorToken;
        if (!pSpot(GLOBALS->IdentifierSymbol(), tokens, operatorToken)) {
            return false;
        }
        value = strip(operatorToken);
        return true;
    }

    static bool _pSpotTheOperator(const std::string& operString, List*& tokens, Object*& value) {
        Identifier* operIdent = Identifier::create(operString);
        List* savedTokens = tokens;
        if (!pSpotValue(GLOBALS->IdentifierSymbol(), operIdent, tokens, value)) {
            return false;
        }
        assert(tokens != savedTokens);
        value = operIdent;
        return true;
    }

    static bool _pPlus(List*& tokens, Object*& value) {
        List* savedTokens = tokens;
        if (!_pSpotTheOperator("+", tokens, value)) {
            return false;
        }
        assert(tokens != savedTokens);
        return true;
    }

    static bool _pMinus(List*& tokens, Object*& value) {
        return _pSpotTheOperator("-", tokens, value);
    }

    static bool _pOpers(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            _pPlus,
            _pMinus
        };
        return pOneOf(parserList, tokens, value);
    }

    static bool _pLhs(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pData, pLiteral
        };
        return pOneOf(parserList, tokens, value);
    }

    static bool _pRhs(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            pData, pLiteral
        };
        if (!pOneOf(parserList, tokens, value)) {
            throwException("Parser", "RHS of operator expression expected", tokenPos(tokens));
        }
        return true;
    }

    bool pBinOpExpr(List*& tokens, Object*& value) {
        static const std::initializer_list<Parser> parserList {
            _pLhs, _pOpers, _pRhs
        };
        Object* partsObj;
        if (!pSeq(parserList, tokens, partsObj)) {
            return false;
        }
        assert(partsObj->isA(TypeId::D_QUEUE));
        Queue* partsQueue = static_cast<Queue*>(partsObj);
        Object* lhs = partsQueue->deq();
        Object* operObj = partsQueue->deq();
        Object* rhs = partsQueue->deq();
        assert(operObj->isA(TypeId::E_IDENTIFIER));
        Identifier* oper = static_cast<Identifier*>(operObj);
        BinOpExpr* binOpExpr = new BinOpExpr(lhs, oper, rhs);
        value = binOpExpr;
        return true;
    }

}

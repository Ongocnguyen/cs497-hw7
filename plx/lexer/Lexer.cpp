#include <cassert>
#include <iostream>
#include <map>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/lexer/Syntax.hpp>
#include <plx/literal/Boolean.hpp>
#include <plx/literal/InputStream.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>

namespace PLX {

    Array* buildEoiToken(Array* position) {
        return new Array({
            GLOBALS->EoiSymbol(),
            GLOBALS->EoiSymbol(),
            position
        });
    }

    Lexer::Lexer() {}

    bool Lexer::tokenize(const std::string& string, List*& tokens, Object*& errorValue) {
        InputStream* inputStream = new InputStream(string);
        this->_state = StateName::INIT;
        return tokenize(inputStream, tokens, errorValue);
    }

    bool Lexer::tokenize(InputStream* inputStream, List*& tokens, Object*& errorValue) {
        Queue* tokenQueue = new Queue();
        std::string lexeme = "";
        char c = 0;
        bool reuseChar = false;
        bool exitLoop = false;
        Array* savedPos;
        while (!exitLoop) {
            if (!reuseChar) {
                c = inputStream->next();
            }
            else {
                reuseChar = false;
            }
            if (_state == StateName::INIT) {
                savedPos = inputStream->position();
            }
#if 0
            extern std::string stateNameString[];
            std::clog << "Lexer::tokenize got char '" << c << "' " << (int)c << "\n"
                << "  state = " << stateNameString[static_cast<int>(_state)] << "\n"
                << "  savedPos = " << savedPos << ", currentPos = " << inputStream->position() << "\n"
                << "  lexeme = '" << lexeme << "', tokenQueue = " << tokenQueue << "\n";
#endif
            Rule rule = _locateRule(c);
            Action action = rule.action;
            switch (action) {
                case Action::KEEP:
                    lexeme += c;
                    break;
                case Action::IGNORE:
                    break;
                case Action::REUSE:
                    reuseChar = true;
                    break;
                case Action::ERROR:
                    errorValue = _error(c, inputStream->position());
                    return false;
                case Action::EXIT:
                    exitLoop = true;
                    break;
                case Action::ESCAPIFY_CHAR:
                    switch (c) {
                        case 'n': c = '\n'; break;
                        case 'r': c = '\r'; break;
                        case 't': c = '\t'; break;
                        default: break;
                    }
                    lexeme += c;
                    break;
                case Action::STRING_ERROR:
                    errorValue = new Array({
                        new String("Unterminated string"),
                        savedPos
                    });
                    return false;
                default:
                    assert(false);
                    break;
            }
            TokenType tokenType = rule.tokenType;
            if (tokenType != TokenType::NONE) {
                Array* token = _createToken(tokenType, lexeme, savedPos);
                tokenQueue->enq(token);
                lexeme = "";
                savedPos = inputStream->position();
            }
            _state = rule.nextState;
        }
        tokenQueue->enq(buildEoiToken(inputStream->position()));
        tokens = tokenQueue->asList();
        return true;
    }

    TypeId Lexer::typeId() const {
        return TypeId::T_LEXER;
    }

    extern std::string DEFAULT_RULE;

    Rule Lexer::_locateRule(char c) const {
        std::vector<Rule>& rules = syntax[_state];
        int n=0;
        while (true) {
            Rule& rule = rules[n];
            if (c == 0 && rule.charSet == "") {
                return rule;
            }
            if (rule.charSet.find(c) < rule.charSet.length()) {
                return rule;
            }
            n++;
            if (n == static_cast<int>(rules.size())) {
                // we've reached the end of the rules
                // return the final rule as the default rule
                return rule;
            }
        }
    }

    Array* Lexer::_error(char c, Array* position) const {
        Array* error = new Array({
            new String("Unexpected character"),
            new String(std::string(1, c)),
            position
        });
        return error;
    }

    bool Lexer::_isReservedWord(const std::string& word) const {
        for (const std::string& reservedWord : reservedWords) {
            if (word == reservedWord) {
                return true;
            }
        }
        return false;
    }

    Array* Lexer::_createToken(TokenType tokenType, std::string& lexeme, Array* pos) const {
        Symbol* tokenTypeSymbol;
        Object* tokenValue;
        switch (tokenType) {
            case TokenType::INT:
                tokenTypeSymbol = GLOBALS->IntegerSymbol();
                tokenValue = new Integer(std::stoi(lexeme));
                break;
            case TokenType::STRING:
                tokenTypeSymbol = GLOBALS->StringSymbol();
                tokenValue = new String(lexeme);
                break;
            case TokenType::SYMBOL:
                tokenTypeSymbol = GLOBALS->SymbolSymbol();
                tokenValue = Symbol::create(lexeme);
                break;
            case TokenType::SPECIAL:
                tokenTypeSymbol = GLOBALS->SpecialSymbol();
                tokenValue = new String(lexeme);
                break;
            case TokenType::WORD:
                if (lexeme == "true") {
                    tokenTypeSymbol = GLOBALS->BooleanSymbol();
                    tokenValue = GLOBALS->True();
                }
                else if (lexeme == "false") {
                    tokenTypeSymbol = GLOBALS->BooleanSymbol();
                    tokenValue = GLOBALS->False();
                }
                else if (lexeme == "nil") {
                    tokenTypeSymbol = GLOBALS->NilSymbol();
                    tokenValue = GLOBALS->NilObject();
                }
                else if (_isReservedWord(lexeme)) {
                    tokenTypeSymbol = GLOBALS->ReservedSymbol();
                    tokenValue = new String(lexeme);
                }
                else {
                    tokenTypeSymbol = GLOBALS->IdentifierSymbol();
                    tokenValue = Identifier::create(lexeme);
                }
                break;
            case TokenType::NONE:
            default:
                assert(false);
                break;
        }
        return new Array({tokenTypeSymbol, tokenValue, pos});
    }

}

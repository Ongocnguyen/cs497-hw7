#include <map>

#include <plx/lexer/Lexer.hpp>
#include <plx/lexer/Syntax.hpp>

namespace PLX {

    std::string DIGITS {"0123456789"};
    std::string UCLETTERS {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    std::string LCLETTERS {"abcdefghijklmnopqrstuvwxyz"};
    std::string WORD_CHARS {UCLETTERS + LCLETTERS + DIGITS + "_"};
    std::string PLUSMINUS {"-+"};
    std::string OPERATORS {PLUSMINUS + "*/<>=!^.%"};
    std::string WHITESPACE {" \n\r\t"};
    std::string QUOTE {"\""};
    std::string BACKSLASH {"\\"};
    std::string EOI {""};
    std::string DEFAULT_RULE {"_DEFAULT_"};

    std::vector<std::string> reservedWords {
        "catch",
        "do",
        "else",
        "end",
        "finally",
        "fun",
        "if",
        "in",
        "let",
        "macro",
        "match",
        "then",
        "throw",
        "try"
    };

    std::unordered_map<StateName, std::vector<Rule>> syntax {
        {
            StateName::INIT, {
                {DIGITS, StateName::INT, Action::KEEP, TokenType::NONE},
                {LCLETTERS, StateName::WORD, Action::KEEP, TokenType::NONE},
                {UCLETTERS, StateName::SYMBOL, Action::KEEP, TokenType::NONE},
                {WHITESPACE, StateName::INIT, Action::IGNORE, TokenType::NONE},
                {QUOTE, StateName::STRING, Action::IGNORE, TokenType::NONE},
                {PLUSMINUS, StateName::NUMSIGN, Action::KEEP, TokenType::NONE},
                {OPERATORS, StateName::OPERATOR, Action::KEEP, TokenType::NONE},
                {EOI, StateName::INIT, Action::EXIT, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::KEEP, TokenType::SPECIAL}
            }
        },
        {
            StateName::NUMSIGN, {
                {DIGITS, StateName::INT, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::WORD}
            }
        },  
        {
            StateName::INT, {
                {DIGITS, StateName::INT, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::INT}
            }
        },
        {
            StateName::WORD, {
                {WORD_CHARS, StateName::WORD, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::WORD}
            }
        },
        {
            StateName::SYMBOL, {
                {WORD_CHARS, StateName::SYMBOL, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::SYMBOL}
            }
        },
        {
            StateName::STRING, {
                {QUOTE, StateName::INIT, Action::IGNORE, TokenType::STRING},
                {BACKSLASH, StateName::ESCAPED_CHAR, Action::IGNORE, TokenType::NONE},
                {EOI, StateName::INIT, Action::STRING_ERROR, TokenType::NONE},
                {DEFAULT_RULE, StateName::STRING, Action::KEEP, TokenType::NONE}
            }
        },
        {
            StateName::ESCAPED_CHAR, {
                {EOI, StateName::INIT, Action::STRING_ERROR, TokenType::NONE},
                {DEFAULT_RULE, StateName::STRING, Action::ESCAPIFY_CHAR, TokenType::NONE}
            }
        },
        {
            StateName::OPERATOR, {
                {OPERATORS, StateName::OPERATOR, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::WORD}
            }
        },
        {
            StateName::OPERATOR, {
                {OPERATORS, StateName::OPERATOR, Action::KEEP, TokenType::NONE},
                {DEFAULT_RULE, StateName::INIT, Action::REUSE, TokenType::WORD}
            }
        }
    };

}

#pragma once

namespace PLX {

    enum class StateName {
        INIT, INT, NUMSIGN, STRING, SYMBOL, WORD, ESCAPED_CHAR, OPERATOR
    };

    // This indicates the action to take on each character.
    enum class Action {
        KEEP, IGNORE, REUSE, ERROR, EXIT, ESCAPIFY_CHAR, STRING_ERROR
    };

    // This indicates what token to create when the rule is triggered.
    enum class TokenType {
        NONE, INT, STRING, SYMBOL, SPECIAL, WORD
    };

    struct Rule {
        std::string charSet;
        StateName nextState;
        Action action;
        TokenType tokenType;
    };

    extern std::unordered_map<StateName, std::vector<Rule>> syntax;
    extern std::vector<std::string> reservedWords;

}

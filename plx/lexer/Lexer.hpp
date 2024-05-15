#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <plx/lexer/Syntax.hpp>
#include <plx/object/Object.hpp>

namespace PLX {

    class Array;
    class InputStream;
    class String;
    class Symbol;

    class Lexer : public Object {
    public:
        Lexer();

        // Unique functions ------------------------------------------------

        bool tokenize(const std::string& string, List*& tokens, Object*& errorValue);
        bool tokenize(InputStream* inputStream, List*& tokens, Object*& errorValue);

        // Overridden functions --------------------------------------------

        TypeId typeId() const override;

    private:
        Array* _error(char c, Array* position) const;
        Rule _locateRule(char c) const;
        Array* _createToken(TokenType tokenType, std::string& lexeme, Array* pos) const;
        bool _isReservedWord(const std::string& word) const;

        StateName _state {StateName::INIT};

    };

}

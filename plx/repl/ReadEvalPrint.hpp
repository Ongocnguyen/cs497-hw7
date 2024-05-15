#pragma once

#include <istream>
#include <string>

#include <plx/object/Object.hpp>

namespace PLX {

    class HashTable;
    class Lexer;
    class Object;
    class VM;

    class ReadEvalPrint : public Object {
    public:

        // ReadEvalPrint();
        ReadEvalPrint(HashTable* argMap);

        // Unique functions ------------------------------------------------

        void rep();
        bool rep(std::istream& istream);
        void readUntilEof(std::istream& istream, std::string& string);
        bool readUntilEol(std::istream& istream, std::string& string);
        bool parseString(std::string string, Queue* exprs);
        void print();
        void evalExprs();
        void parseEvalPrint(std::string string);
        VM* vm();

        // Overridden functions --------------------------------------------

        void markChildren(std::vector<Object*>& objs) override;
        TypeId typeId() const override;

    private:
        HashTable* _argMap;
        Lexer* _lexer;
        VM* _vm;
        Triple* _it;
        Queue* _exprs;
        Queue* _values;
    };

}

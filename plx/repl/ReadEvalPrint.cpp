#include <sstream>

#include <plx/data/HashTable.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/lexer/Lexer.hpp>
#include <plx/literal/Nil.hpp>
#include <plx/parser/P_Primitive.hpp>
#include <plx/repl/ReadEvalPrint.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    bool pAny(List*& tokens, Object*& value);

    ReadEvalPrint::ReadEvalPrint(HashTable* argMap)
        : _argMap {argMap}
        , _lexer {new Lexer()}
        , _vm {new VM()}
        , _exprs {new Queue()}
        , _values {new Queue()}
    {
        Identifier* it = Identifier::create("it");
        _it = _vm->bind(it, GLOBALS->NilObject());
    }

    void ReadEvalPrint::readUntilEof(std::istream& istream, std::string& string) {
        std::ostringstream outputStringStream;
        outputStringStream << istream.rdbuf();
        string = outputStringStream.str();
    }

    bool ReadEvalPrint::readUntilEol(std::istream& istream, std::string& string) {
        if (std::getline(istream, string)) {
            return true;
        }
        return false;
    }

    bool ReadEvalPrint::parseString(std::string string, Queue* exprs) {
        List* tokens;
        Object* error;
        if (!_lexer->tokenize(string, tokens, error)) {
            std::cerr << "Lexer error: " << error << "\n";
            return false;
        }
        while (!isEoi(tokens)) {
            Object* value = GLOBALS->NilObject();
            if (!pAny(tokens, value)) {
                std::cerr << "Parser error: " << value << "\n";
                return false;
            }
            exprs->enq(value);
        }
        return true;
    }

    void ReadEvalPrint::evalExprs() {
        while (!_exprs->isEmpty()) {
            Object* expr = _exprs->deq();
            Object* value;
            value = _vm->evalExpr(expr);
            _values->enq(value);
            _it->setValue(value);
        }
    }

    void ReadEvalPrint::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_argMap);
        objs.push_back(_lexer);
        objs.push_back(_vm);
        objs.push_back(_it);
        objs.push_back(_exprs);
        objs.push_back(_values);
    }

    void ReadEvalPrint::print() {
        while (!_values->isEmpty()) {
            Object* value = _values->deq();
            if (!value->isA(TypeId::L_NIL)) {
                std::cout << value << " :: " << value->typeName() << '\n';
            }
        }
    }

    void ReadEvalPrint::parseEvalPrint(std::string string) {
        try {
            if (string.length() == 0) {
                return;
            }
            if (parseString(string, _exprs)) {
                evalExprs();
                print();
            }
        }
        catch (Object* exception) {
            std::cerr << "PLX exception: " << exception << "\n";
        }
        catch (std::exception& exception) {
            std::cerr << "System exception: " << exception.what() << "\n";
        }
        _vm->clearStacks();
        _values->clear();
        _exprs->clear();
    }

    void ReadEvalPrint::rep() {
        rep(std::cin);
    }

    bool ReadEvalPrint::rep(std::istream& istream) {
        std::string string;
        readUntilEof(istream, string);
        if (string.length() > 0) {
            parseEvalPrint(string);
        }
        return true;
    }

    TypeId ReadEvalPrint::typeId() const {
        return TypeId::T_READEVALPRINT;
    }

    VM* ReadEvalPrint::vm() {
        return _vm;
    }

}

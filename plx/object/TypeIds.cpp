#include <string>
#include <map>

#include <plx/object/TypeIds.hpp>

namespace PLX {

    std::map<TypeId, std::string> TYPE_NAMES {
        {PLX::TypeId::D_ARRAY, "Array"},
        {PLX::TypeId::D_CLOSURE, "Closure"},
        {PLX::TypeId::D_HASHTABLE, "HashTable"},
        {PLX::TypeId::D_INPUTSTREAM, "InputStream"},
        {PLX::TypeId::D_INTERNTABLE, "InternTable"},
        {PLX::TypeId::D_LIST, "List"},
        {PLX::TypeId::D_METHOD, "Method"},
        {PLX::TypeId::D_QUEUE, "Queue"},
        {PLX::TypeId::D_TRIPLE, "Triple"},
        {PLX::TypeId::E_APPLY, "Apply"},
        {PLX::TypeId::E_BINOPEXPR, "BinOp"},
        {PLX::TypeId::E_FUNCTION, "Function"},
        {PLX::TypeId::E_IDENTIFIER, "Identifier"},
        {PLX::TypeId::E_INDEXOP, "IndexOp"},
        {PLX::TypeId::E_IF, "IfThen"},
        {PLX::TypeId::E_LET, "Let"},
        {PLX::TypeId::E_LETIN, "LetIn"},
        {PLX::TypeId::E_MATCH, "Match"},
        {PLX::TypeId::E_NAMEDFUNCTION, "Function"},
        {PLX::TypeId::E_PARENEXPR, "ParenExpr"},
        {PLX::TypeId::E_QUOTE, "Quote"},
        {PLX::TypeId::E_SEQ, "Seq"},
        {PLX::TypeId::E_THROW, "Throw"},
        {PLX::TypeId::E_TRYCATCH, "TryCatch"},
        {PLX::TypeId::L_BOOLEAN, "Boolean"},
        {PLX::TypeId::L_INTEGER, "Integer"},
        {PLX::TypeId::L_NIL, "Nil"},
        {PLX::TypeId::L_PRIMITIVEFUNCTION, "PrimitiveFunction"},
        {PLX::TypeId::L_STRING, "String"},
        {PLX::TypeId::L_SYMBOL, "Symbol"},
        {PLX::TypeId::C_APPLY, "ApplyContin"},
        {PLX::TypeId::C_CATCH, "CatchContin"},
        {PLX::TypeId::C_DROP, "Drop"},
        {PLX::TypeId::C_FINALLY, "FinallyContin"},
        {PLX::TypeId::C_METHOD, "MethodContin"},
        {PLX::TypeId::C_RESTOREENV, "RestoreEnv"},
        {PLX::TypeId::C_THROWCONTIN, "ThrowContin"},
        {PLX::TypeId::C_CONTINUATION, "Continuation"},
        {PLX::TypeId::T_LEXER, "Lexer"},
        {PLX::TypeId::T_OBJECT, "Object"},
        {PLX::TypeId::T_PARSER, "Parser"},
        {PLX::TypeId::T_READEVALPRINT, "ReadEvalPrint"},
        {PLX::TypeId::T_REPL, "REPL"},
        {PLX::TypeId::T_VM, "VM"},
        {PLX::TypeId::Z_ANY, "Any"}
    };

}

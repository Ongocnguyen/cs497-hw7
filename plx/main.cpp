#include <iostream>
#include <unistd.h>

#include <plx/ArgParser.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/repl/ReadEvalPrint.hpp>
#include <plx/repl/REPL.hpp>

void setup() {
    PLX::GLOBALS = new PLX::Globals();
    PLX::GLOBALS->initializeAll();
}

void shutdown() {
    delete PLX::GLOBALS;
}

void logo() {
    std::clog << " ____  _   __  __\n";
    std::clog << "|  _ \\| |  \\ \\/ /\n";
    std::clog << "| |_| | |   \\  /\n";
    std::clog << "|  __/| |_  /  \\\n";
    std::clog << "|_|   |___|/_/\\_\\\n";
}

int main(int argc, char** argv) {
    setup();
    PLX::ArgParser argParser;
    PLX::HashTable* argMap = argParser.parse(argc, argv);
    PLX::GLOBALS->SetArgMap(argMap);
    PLX::GLOBALS->Gc()->setRegistrationType(PLX::RegistrationType::REGULAR);
    PLX::ReadEvalPrint* rep = new PLX::ReadEvalPrint(argMap);
    PLX::GLOBALS->Gc()->addRoot(rep);
    if (!isatty(STDIN_FILENO)) {
        // std::cin is redirected, data comes from a file
        rep->rep();
    }
    else {
        // std::cin is not redirected, enter a REP loop
        logo();
        PLX::REPL* repl = new PLX::REPL(rep);
        PLX::GLOBALS->Gc()->addRoot(repl);
        repl->run();
    }
    shutdown();
    return 0;
}

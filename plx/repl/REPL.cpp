#include <csignal>
#include <unistd.h>

#include <plx/data/Triple.hpp>
#include <plx/gc/GC.hpp>
#include <plx/object/Globals.hpp>
#include <plx/repl/ReadEvalPrint.hpp>
#include <plx/repl/REPL.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    static const std::string PROMPT {"PLX> "};
    static const std::string ESCAPE {":"};

    bool vmInterrupt = false;

    void sigIntHandler(int signum) {
        bool contin = true;
        while (contin) {
            std::cerr << "(C)ontinue, (I)nterrupt, (Q)uit: ";
            std::cerr.flush();
            std::string reply;
            std::getline(std::cin, reply);
            if (reply.length() > 0) {
                char c = reply[0];
                switch (c) {
                    case 'q':
                    case 'Q':
                        exit(signum);
                        break;
                    case 'i':
                    case 'I':
                        vmInterrupt = true;
                        contin = false;
                        break;
                    case 'c':
                    case 'C':
                        contin = false;
                        break;
                }
            }
        }
    }

    REPL::REPL(ReadEvalPrint* rep)
        : _rep {rep}
    {}

    void REPL::handleEscapeString(std::string string) {
        if (string == ":q") {
            this->_continue = false;
        }
        else if (string == ":gc") {
            GLOBALS->Gc()->collect();
        }
        else if (string == ":gcr") {
            GLOBALS->Gc()->showRoots();
        }
        else if (string == ":gcp") {
            GLOBALS->Gc()->showPermanent();
        }
        else if (string == ":gcs") {
            GLOBALS->Gc()->showSpine();
        }
        else if (string == ":env") {
            std::clog << _rep->vm()->environment() << "\n";
        }
        else if (string == ":tron") {
            _rep->vm()->setTrace(true);
        }
        else if (string == ":troff") {
            _rep->vm()->setTrace(false);
        }
        else if (string == ":vm") {
            _rep->vm()->showOn(std::clog);
            std::clog << "\n";
        }
        else if (string == ":help" || string == ":?") {
            std::clog
                << ":q     Quits PLX\n"
                << ":gc    Forces GC mark & sweep\n"
                << ":gcr   Shows GC root objects\n"
                << ":gcp   Shows GC permanent objects\n"
                << ":gcs   Shows GC spine\n"
                << ":tron  Switches VM trace on\n"
                << ":troff Switches VM trace off\n"
                << ":help  Shows this help message\n"
                << ":?     Shows this help message\n"
                ;
        }
        else {
            std::cerr << "Unhandled escape command " << string << "\n";
        }
    }

    void REPL::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_rep);
    }

    void REPL::run() {
        while (this->_continue) {
            std::cout << PROMPT;
            std::string string;
            if (!_rep->readUntilEol(std::cin, string)) {
                break;
            }
            if (string.compare(0, ESCAPE.length(), ESCAPE) == 0)  {
                handleEscapeString(string);
            }
            else {
                signal(SIGINT, sigIntHandler);
                _rep->parseEvalPrint(string);
                signal(SIGINT, SIG_DFL);
            }
        }
        std::cout << std::endl;
    }

    TypeId REPL::typeId() const {
        return TypeId::T_REPL;
    }

}

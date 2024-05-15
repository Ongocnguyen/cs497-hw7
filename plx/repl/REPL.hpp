#include <plx/object/Object.hpp>

namespace PLX {

    class ReadEvalPrint;

    class REPL : public Object {
    public:

        REPL(ReadEvalPrint* rep);

        // Unique functions ------------------------------------------------

        void handleEscapeString(std::string string);
        void run();

        // Overridden functions --------------------------------------------

        void markChildren(std::vector<Object*>& objs) override;
        TypeId typeId() const override;

    private:
        ReadEvalPrint* _rep;
        bool _continue {true};
    };

}

#include <cassert>

#include <plx/expr/Quote.hpp>
#include <plx/vm/VM.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Quote::Quote(Object* expr)
        : _expr {expr}
    {}

    void Quote::eval(VM* vm) {
        vm->pushObj(_expr);
    }

    void Quote::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_expr);
    }

    void Quote::showOn(std::ostream& ostream) const {
        ostream << '\'' << _expr << '\'';
    }

    TypeId Quote::typeId() const {
        return TypeId::E_QUOTE;
    }

}

#include <cassert>

#include <plx/vm/VM.hpp>
#include <plx/expr/Drop.hpp>

namespace PLX {

    Drop::Drop() {}

    void Drop::eval(VM* vm) {
        Object* obj;
        assert(vm->popObj(obj));
    }

    void Drop::showOn(std::ostream& ostream) const {
        ostream << "Drop{}";
    }

    TypeId Drop::typeId() const {
        return TypeId::C_DROP;
    }

}

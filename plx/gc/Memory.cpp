#include <plx/gc/Memory.hpp>

namespace PLX {

    Object* allocate(TypeId typeId, std::initializer_list<Object*> args) {
        (void)typeId;
        (void)args;
        return nullptr;
    }

    void deallocate(Object* obj) {
        (void)obj;
    }

}

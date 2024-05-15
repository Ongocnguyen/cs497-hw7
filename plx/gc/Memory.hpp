#include <unordered_map>

#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class Memory {
    public:
        Object* allocate(TypeId typeId, std::initializer_list<Object*> args);
        void deallocate(Object* obj);
    private:
        std::unordered_map<TypeId, Object*> _recycled;
    };

}

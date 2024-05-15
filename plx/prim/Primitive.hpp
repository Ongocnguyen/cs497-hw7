#include <list>
#include <string>

#include <plx/object/TypeIds.hpp>

namespace PLX {

    class List;
    class VM;

    void argumentCountMismatch(const std::string& name, int nExpectedArgs, int nActualArgs);
    void checkArgTypes(const std::string& name, List* args, std::initializer_list<TypeId> types);

}

#include <list>
#include <string>

namespace PLX {

    class Array;
    class Object;
    class Triple;

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos);
#if 0
    bool containsKeys(Triple* bindings, std::initializer_list<Object*> keys);
#endif
    Array* positionArray(int line, int col, int pos);

}

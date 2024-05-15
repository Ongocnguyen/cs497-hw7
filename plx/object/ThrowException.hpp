#include <string>

namespace PLX {

    class Object;

    void throwException(const std::string& category, const std::string& message, Object* payload);

}

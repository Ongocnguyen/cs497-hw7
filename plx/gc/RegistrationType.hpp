#include <string>

namespace PLX {

    enum class RegistrationType {
        // UNREGISTERED,
        PERMANENT,
        // ROOT,
        REGULAR
    };

    std::string registrationTypeName(RegistrationType regType);

}

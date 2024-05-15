#include <string>

#include <plx/gc/RegistrationType.hpp>

namespace PLX {

    std::string registrationTypeName(RegistrationType regType) {
        static const std::string names[] =
            // {"UNREGISTERED", "PERMANENT", "ROOT", "REGULAR"};
            {"PERMANENT", "REGULAR"};
        if (regType < RegistrationType::PERMANENT || regType > RegistrationType::REGULAR) {
            return "ERROR";
        }
        return names[static_cast<int>(regType)];
    }

}

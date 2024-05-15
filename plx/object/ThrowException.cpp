#include <plx/data/Array.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>

namespace PLX {

        void throwException(const std::string& category, const std::string& message, Object* payload) {
            Array* error = new Array({
                Symbol::create(category),
                new String(message),
                payload
            });
            throw error;
        }

}

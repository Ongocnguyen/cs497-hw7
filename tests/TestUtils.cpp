#include <cassert>

#include <tests/TestUtils.hpp>

#include <plx/data/Array.hpp>
#include <plx/data/Triple.hpp>
#include <plx/literal/Integer.hpp>
#include <plx/literal/Symbol.hpp>

namespace PLX {

    Array* buildToken(const std::string& type, Object* value, int line, int col, int pos) {
        Array* token = new Array({
            Symbol::create(type),
            value,
            positionArray(line, col, pos)
        });
        return token;
    }

#if 0
    bool containsKeys(Triple* bindings, std::initializer_list<Object*> keys) {
        std::clog << "containsKeys bindings = " << bindings << "\n";
        for (Object* key : keys) {
            bool found = false;
            std::clog << "  looking for " << key << "\n";
            while (!bindings->isEmpty()) {
                if (bindings->key() == key) {
                    found = true;
                    break;
                }
                bindings = bindings->next();
            }
            if (!found) {
                std::clog << "  not found\n";
                return false;
            }
        }
        return true;
    }
#endif

    Array* positionArray(int line, int col, int pos) {
        return new Array({
            new Integer(line),
            new Integer(col),
            new Integer(pos)
        });
    }

}

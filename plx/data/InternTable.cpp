#include <vector>

#include <plx/data/InternTable.hpp>

namespace PLX {

    InternTable::InternTable() {}

    void InternTable::add(const std::string key, Object* value) {
        _internMap[key] = value;
    }

    bool InternTable::lookup(const std::string key, Object*& value) {
        std::unordered_map<std::string, Object*>::const_iterator search = _internMap.find(key);
        if (search != _internMap.end()) {
            value = search->second;
            return true;
        }
        return false;
    }

    void InternTable::markChildren(std::vector<Object*>& objs) {
        for (const std::pair<const std::string, Object*>& pair : _internMap) {
            objs.push_back(pair.second);
        }
    }

    void InternTable::showOn(std::ostream& ostream) const {
        ostream << this->typeName() << '{';
        bool firstIter = true;
        for (const std::pair<const std::string, Object*>& pair : _internMap) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                ostream << ", ";
            }
            ostream << pair.first;
        }
        ostream << '}';
    }

    TypeId InternTable::typeId() const {
        return TypeId::D_INTERNTABLE;
    }

}

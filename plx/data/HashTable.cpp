#include <cassert>
#include <vector>

#include <plx/data/Array.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    HashTable::HashTable() {}

    bool HashTable::boolValue() const {
        return _map.size() != 0;
    }

    Object* HashTable::close(Triple* env) {
        HashTable* closedHashTable = new HashTable();
        for (auto iter = _map.begin(); iter != _map.end(); iter++) {
            Object* key = iter->first->close(env);
            Object* value = iter->second->close(env);
            closedHashTable->_map[key] = value;
        }
        return closedHashTable;
    }

    int HashTable::count() const {
        return _map.size();
    }

    bool HashTable::equals(const Object* other) const {
        if (!other->isA(TypeId::D_HASHTABLE)) {
            return false;
        }
        const HashTable* otherHashTable = static_cast<const HashTable*>(other);
        if (count() != otherHashTable->count()) {
            return false;
        }
        for (const auto& pair : _map) {
            Object* key = pair.first;
            Object* value = pair.second;
            Object* otherValue;
            if (!otherHashTable->get(key, otherValue)) {
                return false;
            }
            if (!value->equals(otherValue)) {
                return false;
            }
        }
        return true;
    }

    class HashTableContin : public Object {
    public:
        HashTableContin(int nElems)
            : _nElems {nElems}
        {}
        void eval(VM* vm) override {
            HashTable* hashTable = new HashTable();
            for (int n=0; n<_nElems; n++) {
                Object* key;
                assert(vm->popObj(key));
                Object* value;
                assert(vm->popObj(value));
                hashTable->put(key, value);
            }
            vm->pushObj(hashTable);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "HashTableContin{" << _nElems << "}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        int _nElems;
    };

    void HashTable::eval(VM* vm) {
        vm->pushExpr(new HashTableContin(count()));
        for (auto iter = _map.begin(); iter != _map.end(); iter++) {
            vm->pushExpr(iter->first);
            vm->pushExpr(iter->second);
        }
    }

    bool HashTable::get(Object* key, Object*& value) const {
        auto iter = _map.find(key);
        if (iter != _map.end()) {
            value = iter->second;
            return true;
        }
        return false;
    }

    bool HashTable::index(Object* indexer, Object*& retrievedValue) {
        return get(indexer, retrievedValue);
    }
    
    bool HashTable::isEmpty() const {
        return _map.empty();
    }

    Array* HashTable::keys() const {
        Array* keyArray = new Array(count());
        int n = 0;
        for (auto iter = _map.begin(); iter != _map.end(); iter++, n++) {
            keyArray->set(n, iter->first);
        }
        return keyArray;
    }

    bool HashTable::length(int& len) {
        len = count();
        return true;
    }

    void HashTable::markChildren(std::vector<Object*>& objs) {
        for (auto iter = _map.begin(); iter != _map.end(); iter++) {
            objs.push_back(iter->first);
            objs.push_back(iter->second);
        }
    }

    void HashTable::put(Object* key, Object* value) {
        _map[key] = value;
    }

    void HashTable::removeAllBindings() {
        _map.clear();
    }

    void HashTable::showOn(std::ostream& ostream) const {
        ostream << "#{";
        bool firstIter = true;
        for (auto iter = _map.begin(); iter != _map.end(); iter++) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                ostream << ", ";
            }
            Object* key = iter->first;
            Object* value = iter->second;
            ostream << key << '=' << value;
        }
        ostream << '}';
    }

    TypeId HashTable::typeId() const {
        return TypeId::D_HASHTABLE;
    }

}

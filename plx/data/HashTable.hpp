#pragma once

#include <unordered_map>
#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class Array;
    class Triple;

    class HashTable : public Object {
    public:
        HashTable();

        // Unique functions ------------------------------------------------

        int count() const;
        bool isEmpty() const;
        Array* keys() const;
        bool get(Object* key, Object*& value) const;
        void put(Object* key, Object* value);
        void removeAllBindings();

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        Object* close(Triple* env) override;
        bool equals(const Object* other) const override;
        void eval(VM* vm) override;
        bool index(Object* indexer, Object*& retrievedValue) override;
        bool length(int& len) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        std::unordered_map<Object*, Object*> _map;

    };

}

#pragma once

#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class List;
    class Queue;
    class Triple;

    class Array : public Object {
    public:
        Array(int nElems);
        Array(int nElems, Object* initialValue);
        Array(std::initializer_list<Object*> elems);

        // Unique functions ------------------------------------------------

        int length();
        bool get(int index, Object*& value) ;
        bool set(int index, Object* newElem);

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        Object* close(Triple* env) override;
        bool equals(const Object* other) const override;
        void eval(VM* vm) override;
        List* freeVars(List* freeVars) override;
        bool hashCode(HashCode& hashCode) override;
        bool index(Object* indexer, Object*& retrievedValue) override;
        bool length(int& len) override;
        void markChildren(std::vector<Object*>& objs) override;
        bool match(Object* other, Triple*& bindings) override;
        void showOn(std::ostream& ostream) const override;
        bool toArray(Array*& array) override;
        bool toList(List*& list) override;
        bool toQueue(Queue*& queue) override;
        TypeId typeId() const override;

    private:
        std::vector<Object*> _elems;
    };

}

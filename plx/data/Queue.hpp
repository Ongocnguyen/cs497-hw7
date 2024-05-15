#pragma once

#include <list>
#include <vector>

#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>

namespace PLX {

    class List;

    class Queue : public Object {
    public:
        Queue();
        Queue(std::initializer_list<Object*> elems);

        // Unique functions ------------------------------------------------

        List* asList() const;
        void clear();
        void enq(Object* elem);
        void enq(std::initializer_list<Object*> elems);
        Object* deq();
        bool isEmpty() const;
        int length() const;

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        Object* close(Triple* env) override;
        bool equals(const Object* other) const override;
        void eval(VM* vm) override;
        bool length(int& len) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        bool toArray(Array*& array) override;
        bool toList(List*& list) override;
        bool toQueue(Queue*& queue) override;
        TypeId typeId() const override;

    private:
        List* _head;
        List* _tail;
        int _nElems;

    };

}

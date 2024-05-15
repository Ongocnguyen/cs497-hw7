#include <cassert>
#include <list>
#include <vector>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    Queue::Queue()
        : _head {GLOBALS->EmptyList()}
        , _tail {GLOBALS->EmptyList()}
        , _nElems {0}
    {}

    Queue::Queue(std::initializer_list<Object*> elems)
        : Queue()
    {
        enq(elems);
    }

    List* Queue::asList() const {
        return _head;
    }

    bool Queue::boolValue() const {
        return _nElems != 0;
    }

    void Queue::clear() {
        while (!isEmpty()) {
            deq();
        }
    }

    Object* Queue::close(Triple* env) {
        Queue* closedQueue = new Queue();
        List* elems = _head;
        while (!elems->isEmpty()) {
            Object* elem = elems->first();
            Object* closedElem = elem->close(env);
            closedQueue->enq(closedElem);
            elems = elems->restAsList();
        }
        return closedQueue;
    }

    Object* Queue::deq() {
        if (_nElems == 0) {
            throwException("Queue", "Queue empty", this);
        }
        Object* elem = _head->first();
        _head = _head->restAsList();
        _nElems--;
        return elem;
    }

    void Queue::enq(Object* elem) {
        List* link = new List(elem, GLOBALS->EmptyList());
        if (_nElems == 0) {
            _head = _tail = link;
        }
        else {
            _tail->setRest(link);
            _tail = link;
        }
        _nElems++;
    }

    void Queue::enq(std::initializer_list<Object*> elems) {
        for (Object* elem : elems) {
            enq(elem);
        }
    }

    bool Queue::equals(const Object* other) const {
        if (this == other) {
            return true;
        }
        if (other->isA(TypeId::D_QUEUE)) {
            const Queue* otherQueue = static_cast<const Queue*>(other);
            if (isEmpty()) {
                return otherQueue->isEmpty();
            }
            return _head->equals(otherQueue->_head);
        }
        return false;
    }

    class QueueContin : public Object {
    public:
        QueueContin(int nElems)
            : _nElems {nElems}
        {}
        void eval(VM* vm) override {
            Queue* queue = new Queue();
            for (int n=0; n<_nElems; n++) {
                Object* elem;
                assert(vm->popObj(elem));
                queue->enq(elem);
            }
            vm->pushObj(queue);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "QueueContin{" << _nElems << "}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        int _nElems;
    };

    void Queue::eval(VM* vm) {
        vm->pushExpr(new QueueContin(_nElems));
        List* head = _head;
        int n = 0;
        while (!head->isEmpty()) {
            vm->pushExpr(head->first());
            head = head->restAsList();
            n++;
        }
        assert(_nElems == n);
    }

    bool Queue::isEmpty() const {
        return _nElems == 0;
    }

    int Queue::length() const {
        return _nElems;
    }

    bool Queue::length(int& len) {
        len = _nElems;
        return true;
    }

    void Queue::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_head);
    }

    void Queue::showOn(std::ostream& ostream) const {
        ostream << '~' << _head;
    }

    bool Queue::toArray(Array*& array) {
        return _head->toArray(array);
    }

    bool Queue::toList(List*& list) {
        List* elems = _head;
        Queue* queue = new Queue();
        while (!elems->isEmpty()) {
            queue->enq(elems->first());
            elems = elems->restAsList();
        }
        list = queue->asList();
        return true;
    }

    bool Queue::toQueue(Queue*& queue) {
        queue = this;
        return true;
    }

    TypeId Queue::typeId() const {
        return TypeId::D_QUEUE;
    }

}

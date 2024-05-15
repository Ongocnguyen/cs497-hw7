#include <cassert>
#include <list>
#include <vector>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/vm/VM.hpp> 
#include <plx/literal/Nil.hpp>
#include <plx/literal/String.hpp>
#include <plx/object/Globals.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    List::List()
        : List(GLOBALS->NilObject(), this)
    {}

    List::List(Object* first)
        : List(first, GLOBALS->EmptyList())
    {}


    List::List(Object* first, Object* rest)
        : _first {first}
        , _rest {rest}
    {}

    List* List::create(std::initializer_list<Object*> elems) {
        List* first = GLOBALS->EmptyList();
        List* last = first;
        for (Object* elem : elems) {
            if (first->isEmpty()) {
                first = new List(elem, first);
                last = first;
            }
            else {
                List* link = new List(elem, GLOBALS->EmptyList());
                last->setRest(link);
                last = link;
            }
        }
        return first;
    }

    bool List::boolValue() const {
        return this != GLOBALS->EmptyList();
    }

    Object* List::close(Triple* env) {
        if (isEmpty()) {
            return this;
        }
        return new List(_first->close(env), _rest->close(env));
    }

    bool List::equals(const Object* other) const {
        if (this == other) {
            return true;
        }
        if (other->isA(TypeId::D_LIST)) {
            const List* otherList = static_cast<const List*>(other);
            if (isEmpty()) {
                return otherList->isEmpty();
            }
            return _first->equals(otherList->_first) && _rest->equals(otherList->_rest);
        }
        return false;
    }

    class ListContin : public Object {
    public:
        ListContin() {}
        void eval(VM* vm) override {
            Object* first;
            assert(vm->popObj(first));
            Object* rest;
            assert(vm->popObj(rest));
            List* newList = new List(first, rest);
            vm->pushObj(newList);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "ListContin{}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    };

    void List::eval(VM* vm) {
        if (isEmpty()) {
            vm->pushObj(this);
        }
        else {
            vm->pushExpr(new ListContin());
            vm->pushExpr(_first);
            vm->pushExpr(_rest);
        }
    }

    Object* List::first() {
        if (isEmpty()) {
            throwException("List", "List is empty", this);
        }
        return _first;
    }

    List* List::freeVars(List* freeVars) {
        List* list = this;
        while (!list->isEmpty()) {
            freeVars = list->_first->freeVars(freeVars);
            if (_rest->isA(TypeId::D_LIST)) {
                list = static_cast<List*>(list->_rest);
            }
            else {
                list = list->_rest->freeVars(freeVars);
                break;
            }
        }
        return freeVars;
    }

    bool List::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        List* list = this;
        HashCode hashCode1 = typeHashCode;
        while (!list->isEmpty()) {
            HashCode hashCode2;
            if (!list->_first->hashCode(hashCode2)) {
                return false;
            }
            hashCode1 = hashRotateLeft(hashCode1) ^ hashCode2;
            if (_rest->isA(TypeId::D_LIST)) {
                list = static_cast<List*>(list->_rest);
            }
            else {
                if (!list->_rest->hashCode(hashCode2)) {
                    return false;
                }
                hashCode1 = hashRotateLeft(hashCode1) ^ hashCode2;
                break;
            }
        }
        hashCode = hashCode1;
        return true;
    }

    bool List::isEmpty() const {
        return this == GLOBALS->EmptyList();
    }

    int List::length() const {
        if (isEmpty()) {
            return 0;
        }
        if (_rest->isA(TypeId::D_LIST)) {
            List* restList = static_cast<List*>(_rest);
            return 1 + restList->length();
        }
        return 2;
    }

    bool List::length(int& len) {
        len = length();
        return true;
    }

    List* List::locate(Object* obj) {
        List* list = this;
        while (!list->isEmpty()) {
            if (*(list->_first) == *obj) {
                break;
            }
            if (list->_rest->isA(TypeId::D_LIST)) {
                list = static_cast<List*>(list->_rest);
            }
            else {
                list = new List(list->_rest);
            }
        }
        return list;
    }

    void List::markChildren(std::vector<Object*>& objs) {
        if (isEmpty()) {
            return;
        }
        objs.push_back(_first);
        objs.push_back(_rest);
    }

    bool List::match(Object* other, Triple*& bindings) {
        if (!other->isA(TypeId::D_LIST)) {
            return false;
        }
        List* otherList = static_cast<List*>(other);
        if (isEmpty()) {
            return otherList->isEmpty();
        }
        if (otherList->isEmpty()) {
            return false;
        }
        if (!_first->match(otherList->_first, bindings)) {
            return false;
        }
        return _rest->match(otherList->_rest, bindings);
    }

    Object* List::rest() {
        if (isEmpty()) {
            throwException("List", "List is empty", this);
        }
        return _rest;
    }

    List* List::restAsList() const {
        assert(_rest->isA(TypeId::D_LIST));
        return static_cast<List*>(_rest);
    }

    List* List::reverse() {
        List* reversed = GLOBALS->EmptyList();
        List* list = this;
        while (!list->isEmpty()) {
            reversed = new List(list->_first, reversed);
            Object* restObj = list->_rest;
            if (restObj->isA(TypeId::D_LIST)) {
                List* restList = static_cast<List*>(restObj);
                list = restList;
            }
            else {
                reversed = new List(restObj, reversed);
                break;
            }
        }
        return reversed;
    }

    Object* List::second() {
        return restAsList()->_first;
    }

    Object* List::third() {
        return restAsList()->restAsList()->_first;
    }

    void List::setFirst(Object* first) {
        if (isEmpty()) {
            throwException("List", "Operation not allowed on empty list", this);
        }
        _first = first;
    }

    void List::setRest(Object* rest) {
        if (isEmpty()) {
             throwException("List", "Operation not allowed on empty list", this);
        }
        _rest = rest;
    }

    void List::showOn(std::ostream& ostream) const {
        showOnWith(ostream, "[", ", ", "]");
    }

    void List::showOnWith(std::ostream& ostream, const std::string& open, const std::string& sep, const std::string& close) const {
        const List* list = this;
        ostream << open;
        bool firstIter = true;
        while (!list->isEmpty()) {
            if (firstIter) {
                firstIter = false;
            }
            else {
                ostream << sep;
            }
            ostream << list->_first;
            Object* restObj = list->_rest;
            if (restObj->isA(TypeId::D_LIST)) {
                List* restList = static_cast<List*>(restObj);
                list = restList;
            }
            else {
                ostream << " | " << restObj;
                break;
            }
        }
        ostream << close;
    }

    bool List::toArray(Array*& array) {
        List* list = this;
        int nElems = length();
        array = new Array(nElems);
        for (int n=0; n<nElems; n++) {
            Object* elem = list->_first;
            array->set(n, elem);
            Object* restObj = list->_rest;
            if (restObj->isA(TypeId::D_LIST)) {
                list = static_cast<List*>(restObj);
            }
            else {
                list = new List(restObj);
            }
        }
        return true;
    }

    bool List::toList(List*& list) {
        list = this;
        return true;
    }
    
    bool List::toQueue(Queue*& queue) {
        queue = new Queue();
        List* list = this;
        while (!list->isEmpty()) {
            queue->enq(list->_first);
            Object* restObj = list->_rest;
            if (restObj->isA(TypeId::D_LIST)) {
                List* restList = static_cast<List*>(restObj);
                list = restList;
            }
            else {
                queue->enq(restObj);
                break;
            }
        }
        return true;
    }
    
    TypeId List::typeId() const {
        return TypeId::D_LIST;
    }

}

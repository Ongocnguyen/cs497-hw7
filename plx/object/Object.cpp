#include <cassert>
#include <list>
#include <sstream>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Queue.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/gc/GC.hpp>
#include <plx/literal/String.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    std::ostream& operator<<(std::ostream& ostream, const PLX::Object* object) {
        object->showOn(ostream);
        return ostream;
    }

    // This is set to an actual value in Globals.
    GC* Object::_gc = nullptr;

    bool Object::operator==(const Object& rhs) const {
        return this->equals(&rhs);
    }

    bool Object::operator==(const Object* rhs) const {
        return this->equals(rhs);
    }

    bool Object::operator!=(const Object& rhs) const {
        return !this->equals(&rhs);
    }

    void Object::setGC(GC* gc) {
        Object::_gc = gc;
    }

    Object::Object() {
        Object::_gc->registerObject(this);
    }

    void Object::apply(VM* vm, List* arguments) {
        (void)vm;
        (void)arguments;
        throwException("Object::apply", "Object is not applyable", this);
    }

    bool Object::boolValue() const {
        return true;
    }

    Object* Object::close(Triple* env) {
        (void)env;
        return this;
    }

    void Object::displayOn(std::ostream& ostream) const {
        showOn(ostream);
    }

    bool Object::equals(const Object* other) const {
        // Cast the pointers to void* to ensure that the overloaded
        // operator==(Object*) functino is not called.
        const void* thisPtr = static_cast<const void*>(this);
        const void* otherPtr = static_cast<const void*>(other);
        return thisPtr == otherPtr;
    }

    void Object::eval(VM* vm) {
        vm->pushObj(this);
    }

    List* Object::freeVars(List* freeVars) {
        return freeVars;
    }

    List* Object::freeVars(std::initializer_list<Object*> objs, List* freeVars) {
        for (Object* obj : objs) {
            freeVars = obj->freeVars(freeVars);
        }
        return freeVars;
    }

    bool Object::hashCode(HashCode& hashCode) {
        (void)hashCode;
        // void throwException(const std::string& category, const std::string& message, Object* payload);
        Array* payload = new Array({this, this->typeSymbol()});
        throwException("Object", "Object is not hashable", payload);
        return false;
    }

    bool Object::index(Object* indexer, Object*& retrievedValue) {
        (void)indexer;
        (void)retrievedValue;
        return false;
    }

    bool Object::isA(TypeId typeId) const {
        return (this->typeId()) == typeId;
    }

    bool Object::isMarked() const {
        return _isMarked;
    }

    bool Object::length(int& len) {
        (void)len;
        return false;
    }

    void Object::mark(std::vector<Object*>& objs) {
        if (!_isMarked) {
            _isMarked = true;
            markChildren(objs);
        }
    }

    // See also Object::unmark()
    bool Object::setMark(bool markValue) {
        bool isMarked = _isMarked;
        _isMarked = markValue;
        return isMarked;
    }

    bool Object::match(Object* other, Triple*& bindings) {
        (void)bindings;
        return this->equals(other);
    }

    void Object::markChildren(std::vector<Object*>& objs) {
        (void)objs;
    }

    bool Object::plus(Object* other, Object*& value) {
        (void)other;
        (void)value;
        return false;
    }

    bool Object::minus(Object* other, Object*& value) {
        (void)other;
        (void)value;
        return false;
    }

    bool Object::times(Object* other, Object*& value) {
        (void)other;
        (void)value;
        return false;
    }

    bool Object::divide(Object* other, Object*& value) {
        (void)other;
        (void)value;
        return false;
    }

    bool Object::percent(Object* other, Object*& value) {
        (void)other;
        (void)value;
        return false;
    }

    void Object::showOn(std::ostream& ostream) const {
        ostream << "Object@" << static_cast<const void*>(this);
    }

    bool Object::toArray(Array*& array) {
        (void)array;
        return false;
    }

    bool Object::toList(List*& list) {
        (void)list;
        return false;
    }

    bool Object::toQueue(Queue*& queue) {
        (void)queue;
        return false;
    }

    std::string Object::toString() {
        std::ostringstream ss;
        ss << this;
        return ss.str();
    }

    TypeId Object::typeId() const {
        return TypeId::T_OBJECT;
    }

    std::string Object::typeName() const {
        return TYPE_NAMES[typeId()];
    }

    Symbol* Object::typeSymbol() const {
        return Symbol::create(typeName());
    }

}

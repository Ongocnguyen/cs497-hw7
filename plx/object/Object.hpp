#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include <plx/object/HashCode.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    enum class TypeId;

    class Array;
    class GC;
    class List;
    class Queue;
    class Symbol;
    class Triple;
    class VM;

    class Object {
    public:
        static void setGC(GC* gc);

        Object();
        virtual ~Object() = default;

        virtual void apply(VM* vm, List* arguments);
        virtual bool boolValue() const;  // DEPRECATED
        virtual Object* close(Triple* env);
        virtual void displayOn(std::ostream& ostream) const;
        virtual bool equals(const Object* other) const;
        virtual void eval(VM* vm);
        virtual List* freeVars(List* freeVars);
        virtual bool hashCode(HashCode& hashCode);
        virtual bool index(Object* indexer, Object*& retrievedValue);
        virtual bool length(int& len);
        virtual void markChildren(std::vector<Object*>& objs);
        virtual bool match(Object* other, Triple*& bindings);
        virtual TypeId typeId() const;

        // type conversions
        virtual void showOn(std::ostream& ostream) const;
        virtual bool toArray(Array*& array);
        virtual bool toList(List*& list);
        virtual bool toQueue(Queue*& queue);

        // operator support
        virtual bool plus(Object* other, Object*& value);
        virtual bool minus(Object* other, Object*& value);
        virtual bool times(Object* other, Object*& value);
        virtual bool divide(Object* other, Object*& value);
        virtual bool percent(Object* other, Object*& value);

        static List* freeVars(std::initializer_list<Object*> objs, List* freeVars);

        bool isA(TypeId typeId) const;
        bool isMarked() const;
        void mark(std::vector<Object*>& objs);
        bool setMark(bool markValue);

        std::string toString();
        std::string typeName() const;
        Symbol* typeSymbol() const;

        bool operator==(const Object& rhs) const;
        bool operator==(const Object* rhs) const;
        bool operator!=(const Object& rhs) const;

    private:
        static GC* _gc;

        mutable bool _isMarked {false};
    };

    // This operator can't be a member function because the first
    // parameter is an ostream and not an Object.
    std::ostream& operator<<(std::ostream& ostream, const PLX::Object* object);
}

namespace std {

    // The std::unordered_map class uses these functions.

    template <>
    struct hash<PLX::Object*> {
        size_t operator()(PLX::Object* obj) const {
            PLX::HashCode hashCode;
            assert(obj->hashCode(hashCode));
            return hashCode;
        }
    };

    template <>
    struct equal_to<PLX::Object*> {
        bool operator()(const PLX::Object* obj1, const PLX::Object* obj2) const {
            const void* vobj1 = static_cast<const void*>(obj1);
            const void* vobj2 = static_cast<const void*>(obj2);
            if (vobj1 == vobj2) {
                return true;
            }
            return obj1->equals(obj2);
        }
    };
}

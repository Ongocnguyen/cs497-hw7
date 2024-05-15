#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Triple : public Object {
    public:
        Triple();
        Triple(Object* key, Object* value);
        Triple(Object* key, Object* value, Triple* next);

        // Unique functions ------------------------------------------------

        bool isEmpty() const;
        Object* key() const;
        Object* value() const;
        Triple* next() const;
        void setValue(Object* value);
        void setNext(Triple* next);
        Triple* locate(const Object* key);
        bool lookup(const Object* key, Object*& value);
        bool matchLocate(Object* object, Object*& value, Triple*& env);
        void showOnWith(std::ostream& ostream, const std::string& separator) const;

        // Overridden functions --------------------------------------------

        bool boolValue() const override;
        bool equals(const Object* other) const override;
        void eval(VM* vm) override;
        bool length(int& len) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _key;
        Object* _value;
        Triple* _next;

    };

}

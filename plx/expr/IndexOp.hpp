#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class List;
    class VM;

    class IndexOp : public Object {
    public:
        IndexOp(Object* collection, Object* index);

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _collection;
        Object* _index;
    };

}

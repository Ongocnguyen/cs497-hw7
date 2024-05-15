#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class List;
    class VM;

    class Apply : public Object {
    public:
        Apply(Object* abstractionObj, List* arguments);

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _abstractionObj;
        List* _arguments;
    };

}

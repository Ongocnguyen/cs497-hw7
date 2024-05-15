#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class VM;

    class If : public Object {
    public:
        If(Object* cond, Object* conseq, Object* alt);

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _cond;
        Object* _conseq;
        Object* _alt;
    };

}

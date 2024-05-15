#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class List;

    class Do : public Object {
    public:
        Do(List* exprs);

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        List* _exprs;
        List* _reverseExprs;
     };

}

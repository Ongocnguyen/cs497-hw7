#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Quote : public Object {
    public:

        Quote(Object* expr);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _expr;

    };

}

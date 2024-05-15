#pragma once

#include <plx/object/Object.hpp>

namespace PLX {

    class Finally;
    class List;

    class TryCatch : public Object {
    public:
        TryCatch(Object* expr, Triple* catchRules);
        TryCatch(Object* expr, Triple* catchRules, Object* finallyExpr);

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _expr;
        Triple* _catchRules;
        Object* _finallyExpr;
        Finally* _finally;
    };

}

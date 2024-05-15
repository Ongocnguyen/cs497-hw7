#include <plx/object/Object.hpp>

namespace PLX {

    class Finally : public Object {
    public:
        Finally(Object* expr);

        // Unique functions ------------------------------------------------

        Object* expr();

        // Overridden functions --------------------------------------------

        Object* close(Triple* env) override;
        void eval(VM* vm) override;
        List* freeVars(List* freeVars) override;
        void markChildren(std::vector<Object*>& objs) override;
        bool match(Object* other, Triple*& bindings) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _expr;
    };

}

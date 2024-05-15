#include <plx/object/Object.hpp>

namespace PLX {

    class Catch : public Object {
    public:
        Catch(Triple* catchRules);

        // Unique functions ------------------------------------------------

        bool catchException(Object* exception, VM* vm);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        List* freeVars(List* freeVars) override;
        //static List* freeVars(std::initializer_list<Object*> objs, List* freeVars);
        void markChildren(std::vector<Object*>& objs) override;
        bool match(Object* other, Triple*& bindings) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Triple* _rules;
    };

}

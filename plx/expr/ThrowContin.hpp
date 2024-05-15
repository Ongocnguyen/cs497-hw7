#include <plx/object/Object.hpp>

namespace PLX {

    class ThrowContin : public Object {
    public:

        ThrowContin();

        // Unique functions ------------------------------------------------

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Object* _exception;
        bool _hasException = false;

    };

}

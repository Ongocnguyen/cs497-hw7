#include <plx/object/Object.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    class VM;

    class Drop : public Object {
    public:
        Drop();
        void eval(VM* vm) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;
    };

}

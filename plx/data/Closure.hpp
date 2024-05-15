#pragma once

#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class List;
    class Triple;
    class Function;

    class Closure : public Object {
    public:
        Closure(Function* function, Triple* lexicalEnvironment);

        // Overridden functions --------------------------------------------

        void apply(VM* vm, List* arguments) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        Function* _function;
        Triple* _lexicalEnvironment;
    };

}

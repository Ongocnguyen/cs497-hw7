#pragma once

#include <plx/data/Triple.hpp>
#include <plx/object/Object.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    class RestoreEnv : public Object {
    public:
        // Unique functions ------------------------------------------------

        static void pushContin(Triple* env, VM* vm);

        // Overridden functions --------------------------------------------

        void eval(VM* vm) override;
        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        RestoreEnv(Triple* savedEnv);
        Triple* _savedEnv;
    };

};

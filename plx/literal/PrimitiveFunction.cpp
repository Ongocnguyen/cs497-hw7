#include <cassert>
#include <cstdint>

#include <plx/data/List.hpp>
#include <plx/vm/VM.hpp>
#include <plx/literal/PrimitiveFunction.hpp>
#include <plx/object/HashCode.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    PrimitiveFunction::PrimitiveFunction(const std::string& name, Primitive primitive)
        : _name {name}
        , _primitive {primitive}
    {}

    class PrimApplyContin : public Object {
    public:
        PrimApplyContin(List* arguments)
            : _arguments {arguments}
         {}
        void eval(VM* vm) override {
            Object* abstractionValue;
            assert(vm->popObj(abstractionValue));
            abstractionValue->apply(vm, _arguments);
        }
        void markChildren(std::vector<Object*>& objs) override {
            objs.push_back(_arguments);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "ApplyContin{}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    private:
        List* _arguments;
    };

    void PrimitiveFunction::apply(VM* vm, List* arguments) {
        _primitive(vm, arguments);
    }

    bool PrimitiveFunction::hashCode(HashCode& hashCode) {
        static const int typeHashCode = std::hash<int>{}(static_cast<int>(typeId()));
        static const std::hash<Primitive> primHasher;
        hashCode = typeHashCode ^ (primHasher(_primitive) << 1);
        return true;
    }

    std::string PrimitiveFunction::name() {
        return _name;
    }

    void PrimitiveFunction::showOn(std::ostream& ostream) const {
        ostream << typeName()
                << '{' << _name << '@'
                << reinterpret_cast<void*>(_primitive)
                << '}';
    }

    TypeId PrimitiveFunction::typeId() const {
        return TypeId::L_PRIMITIVEFUNCTION;
    }

}

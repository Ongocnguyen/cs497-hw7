#include <cassert>
#include <vector>

#include <plx/data/Array.hpp>
#include <plx/data/List.hpp>
#include <plx/expr/IndexOp.hpp>
#include <plx/literal/Symbol.hpp>
#include <plx/object/Object.hpp>
#include <plx/object/ThrowException.hpp>
#include <plx/object/TypeIds.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    IndexOp::IndexOp(Object* collection, Object* index)
        : _collection {collection}
        , _index {index}
    {}

    Object* IndexOp::close(Triple* env) {
        return new IndexOp(_collection->close(env), _index->close(env));
    }

    class IndexOpContin : public Object {
    public:
        IndexOpContin() {}
        void eval(VM* vm) override {
            Object* collectionObj;
            assert(vm->popObj(collectionObj));
            Object* indexObj;
            assert(vm->popObj(indexObj));
            Object* retrievedValue;
            if (!collectionObj->index(indexObj, retrievedValue)) {
                throwException("Index", "Object is not indexable", 
                    new Array({collectionObj, collectionObj->typeSymbol()}));
            }
            vm->pushObj(retrievedValue);
        }
        void showOn(std::ostream& ostream) const override {
            ostream << "IndexOpContin{}";
        }
        TypeId typeId() const override {
            return TypeId::C_CONTINUATION;
        }
    };

    void IndexOp::eval(VM* vm) {
        static IndexOpContin* indexOpContin = new IndexOpContin();
        vm->pushExpr(indexOpContin);
        vm->pushExpr(_collection);
        vm->pushExpr(_index);
    }

    void IndexOp::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_collection);
        objs.push_back(_index);
    }

    void IndexOp::showOn(std::ostream& ostream) const {
        ostream << _collection << '[' << _index << ']';
    }

    TypeId IndexOp::typeId() const {
        return TypeId::E_INDEXOP;
    }

}

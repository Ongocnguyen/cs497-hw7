#include <plx/expr/Catch.hpp>
#include <plx/expr/Finally.hpp>
#include <plx/expr/ThrowContin.hpp>
#include <plx/vm/VM.hpp>

namespace PLX {

    ThrowContin::ThrowContin() {}

    void ThrowContin::eval(VM* vm) {
        Object* exception;
        if (!_hasException) {
            assert(vm->popObj(exception));
            _exception = exception;
            _hasException = true;
        }
        else {
            exception = _exception;
        }
        Object* expr;
        while (vm->popExpr(expr)) {
            switch (expr->typeId()) {
                case TypeId::C_CATCH:
                    {
                        Catch* catchExpr = static_cast<Catch*>(expr);
                        if (catchExpr->catchException(exception, vm)) {
                            return;
                        }
                    }
                    break;
                case TypeId::C_FINALLY:
                    {
                        vm->pushExpr(this);
                        Finally* finally = static_cast<Finally*>(expr);
                        vm->pushExpr(finally->expr());
                        return;
                    }
                    break;
                default:
                    break;
            }
        }
        throw _exception;
    }

    void ThrowContin::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_exception);
    }

    void ThrowContin::showOn(std::ostream& ostream) const {
        ostream << "ThrowContin{";
        if (_hasException) {
            ostream << _exception;
        }
        ostream << '}';
    }

    TypeId ThrowContin::typeId() const {
        return TypeId::C_THROWCONTIN;
    }

}

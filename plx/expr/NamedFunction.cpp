#include <plx/data/Closure.hpp>
#include <plx/data/List.hpp>
#include <plx/data/Triple.hpp>
#include <plx/vm/VM.hpp>
#include <plx/expr/Function.hpp>
#include <plx/expr/Identifier.hpp>
#include <plx/expr/Let.hpp>
#include <plx/expr/NamedFunction.hpp>
#include <plx/object/TypeIds.hpp>

namespace PLX {

    NamedFunction::NamedFunction(Identifier* name, Function* function)
        : _name {name}
        , _function {function}
        , _let {new Let(new Triple(_name, _function))}
    {}

    Object* NamedFunction::close(Triple* env) {
        Function* closedFunction = static_cast<Function*>(_function->close(env));
        return new Let(new Triple(_name, closedFunction));
    }

    void NamedFunction::eval(VM* vm) {
        return _let->eval(vm);
    }

    void NamedFunction::markChildren(std::vector<Object*>& objs) {
        objs.push_back(_let);
    }

    void NamedFunction::showOn(std::ostream& ostream) const {
        ostream << "fun "
                << _name;
        _function->showRulesOn(ostream);
    }

    TypeId NamedFunction::typeId() const {
        return TypeId::E_NAMEDFUNCTION;
    }

}
#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>

#include <plx/object/Object.hpp>

namespace PLX {

    class InternTable : public Object {
    public:

        InternTable();

        // Unique functions ------------------------------------------------

        void add(const std::string key, Object* value);
        bool lookup(const std::string key, Object*& value);

        // Overridden functions --------------------------------------------

        void markChildren(std::vector<Object*>& objs) override;
        void showOn(std::ostream& ostream) const override;
        TypeId typeId() const override;

    private:
        struct HashString {
            std::size_t operator()(const std::string& str) const {
                return std::hash<std::string>{}(str);
            }
        };

        std::unordered_map<const std::string, Object*, HashString> _internMap;
    };

}

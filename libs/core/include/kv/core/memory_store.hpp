#pragma once
#include <kv/core/store.hpp>
#include <unordered_map>

namespace kv{
class MemoryStore: public Store{
private:
    std::unordered_map<std::string,std::string> data_;
public:
    void set(std::string_view key,std::string_view value);
    std::optional<std::string> get(std::string_view key) const;
    bool del(std::string_view key);
    bool contains(std::string_view key) const;
};

}
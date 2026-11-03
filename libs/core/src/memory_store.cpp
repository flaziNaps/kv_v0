#include <kv/core/memory_store.hpp>

//set命令实现
void kv::MemoryStore::set(std::string_view key,std::string_view value){
    data_[std::string(key)] = std::string(value);
}

//get命令实现
std::optional<std::string> kv::MemoryStore::get(std::string_view key) const{
    auto it = data_.find(std::string(key));
    if(it==data_.end()) return std::nullopt;
    return it->second;
}

//del命令实现
bool kv::MemoryStore::del(std::string_view key){
    return data_.erase(std::string(key)) > 0;
}

//contains命令实现
bool kv::MemoryStore::contains(std::string_view key) const{
    return data_.find(std::string(key)) != data_.end();
}
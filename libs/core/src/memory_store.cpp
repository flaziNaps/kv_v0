#include <iostream>
#include <kv/core/memory_store.hpp>


void kv::MemoryStore::set(std::string_view key,std::string_view value){
    std::cout << "TODO" << std::endl;
}

std::optional<std::string> kv::MemoryStore::get(std::string_view key) const{
    std::cout << "TODO" << std::endl;
}

bool kv::MemoryStore::del(std::string_view key){
    std::cout << "TODO" << std::endl;
}

bool kv::MemoryStore::contains(std::string_view key) const{
    std::cout << "TODO" << std::endl;
}
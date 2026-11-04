#include <kv/core/memory_store.hpp>
#include <iostream>
int main()
{
   

    std::unordered_map<std::string, std::string> m;
    std::cout << m.size() << "\n";   // 预测：?
    auto v = m["nokey"];             // 读一个不存在的键
    std::cout << "[" << v << "]\n";  // 预测：?
    std::cout << m.size() << "\n";   // 预测：?

}

#include <kv/core/memory_store.hpp>

int main()
{
    // 至少验证 MemoryStore 已经能实例化（不再是抽象类）、且链接到 KV::Core 成功。
    kv::MemoryStore store;
    return 0;
}

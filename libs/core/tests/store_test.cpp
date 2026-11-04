// MemoryStore 单元测试：直接测 store 的返回值语义（不经过 CLI，不打印提示）。
// 对应 require.md 的 1、2、6 条里"存储逻辑"那部分。
#include <kv/core/memory_store.hpp>
#include <iostream>
#include <optional>
#include <string>

static int failures = 0;

// 一个断言：条件不成立就记一次失败。
// 不用 <cassert> 的 assert，因为 assert 在 Release（-DNDEBUG）下会被编译掉，测试形同虚设。
void check(bool ok, const char* what)
{
    if (!ok) {
        std::cout << "FAIL: " << what << "\n";
        ++failures;
    }
}

int main()
{
    kv::MemoryStore store;

    // 需求 1：set 后 get 得到原值
    store.set("a", "1");
    check(store.get("a") == std::optional<std::string>{"1"}, "set 后 get 得到原值");

    // 需求 1：get 不存在的 key 返回 nullopt
    check(store.get("nope") == std::nullopt, "get 不存在的 key 返回 nullopt");

    // 需求 1：重复 set 覆盖旧值
    store.set("a", "2");
    check(store.get("a") == std::optional<std::string>{"2"}, "重复 set 覆盖旧值");

    // 需求 2：del 存在的 key 返回 true，且删后 get 不到
    check(store.del("a") == true, "del 存在的 key 返回 true");
    check(store.get("a") == std::nullopt, "del 之后 get 不到");

    // 需求 2 + 补充 8：del 不存在的 key 返回 false；删两次，第二次也 false
    check(store.del("nope") == false, "del 不存在的 key 返回 false");
    store.set("b", "x");
    store.del("b");
    check(store.del("b") == false, "重复 del 同一个 key 返回 false");

    // 需求 6 + 补充 10：contains 存在的为 true，不存在的为 false
    store.set("c", "y");
    check(store.contains("c") == true, "contains 存在的 key 为 true");
    check(store.contains("nope") == false, "contains 不存在的 key 为 false");

    // 需求 4：showAll 至少不崩溃（它打印到 stdout，输出内容交给 CLI 集成测试去验证）
    store.showAll();

    if (failures == 0) {
        std::cout << "All store tests passed.\n";
        return 0;
    }
    std::cout << failures << " store test(s) failed.\n";
    return 1;
}

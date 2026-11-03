#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace kv {

// 只声明"能做什么"，不写"怎么做"，具体实现留给 MemoryStore / FileStore。
// 这个类不能直接 new（因为还有未实现的纯虚函数），只能被继承。
class Store
{
public:
    // 【为什么必须有虚析构】以后你会在 main 里这样用：
    //     std::unique_ptr<Store> s = std::make_unique<MemoryStore>();
    // 销毁时走的是基类指针，只有把析构函数声明成 virtual，
    // 才会真正调用 MemoryStore 的析构，否则派生类的资源不会释放（内存泄漏）。
    virtual ~Store() = default;

    // 【为什么删除拷贝】接口对象只该通过指针/引用使用。
    // 如果允许按值拷贝，把一个 MemoryStore 拷进一个 Store 变量时，
    // 只会拷到"基类那部分"，派生类的成员（比如那个 map）会被丢掉——这叫对象切片（slicing）。
    Store() = default;
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;

    // 写入：key 已存在就覆盖，不存在就新建。
    // 参数用 string_view：它是"只读引用"，不拷贝字符串本身，
    // 所以调用方传 std::string 或字符串字面量 "abc" 都行，零开销。
    virtual void set(std::string_view key, std::string_view value) = 0;

    // 读取：用 std::optional 表示"可能没有值"。
    // 查到就返回 optional{value}，查不到返回 std::nullopt。
    // 这比"用空字符串当找不到""抛异常"都更诚实、更安全。
    // 返回值用 std::string（拥有数据）：因为这个值是在函数内部生成的，
    // 它必须能活到函数结束之后，所以得自己拥有一块内存，不能返回 view。
    virtual std::optional<std::string> get(std::string_view key) const = 0;

    // 删除：返回 true 表示真的删掉了一个 key，false 表示本来就不存在。
    virtual bool del(std::string_view key) = 0;

    // 判断是否存在。const 表示"这个方法不修改存储"，所以能在 const 对象上调用。
    virtual bool contains(std::string_view key) const = 0;

    //展示现在的所有数据
    virtual void showAll() const = 0;
};

} // namespace kv

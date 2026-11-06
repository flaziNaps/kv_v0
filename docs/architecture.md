# 工程结构

当前项目提供内存 KV 存储和交互式 CLI，使用 C++20、CMake 与 Ninja。

## 模块边界

- `libs/core/include/kv/core/`：公开存储 API。
- `libs/core/src/`：内存存储实现；其他模块通过 `KV::Core` 链接，不直接编译实现文件。
- `libs/core/tests/`：GoogleTest 核心功能测试。
- `libs/core/benchmarks/`：Google Benchmark，测量读取和覆盖写。
- `apps/cli/main.cpp`：将标准输入交给命令循环。
- `apps/cli/cli.cpp`、`cli.hpp`：命令解析、输出和 `kv::cli::run(std::istream&)`。
- `apps/cli/tests/`：GoogleTest 在进程内调用同一命令循环，用字符串流输入、捕获标准输出，覆盖原 Bash 脚本的行为用例。
- `cmake/Dependencies.cmake`：按开关获取固定版本的 GoogleTest 和 Google Benchmark。

依赖方向为 `kv_cli -> kv_cli_commands -> KV::Core`。CLI 测试链接 `kv_cli_commands`，核心测试和 benchmark 链接 `KV::Core`；测试分别链接各自框架的 main target。核心库不依赖测试框架。

## 构建约定

- 根 CMake 负责工程声明、依赖开关与子目录组装。
- 模块显式列出源文件，通过 target 传播头文件路径及语言要求。
- `BUILD_TESTING` 控制 GoogleTest；`BUILD_BENCHMARKS` 独立控制性能测试。关闭的框架不会被下载和编译。
- Debug 默认只开单元测试；Release 默认同时开启单元测试和性能测试。
- GoogleTest 用例通过 `gtest_discover_tests` 注册到 CTest；性能测试独立执行，不注册为 CTest 用例。
- GoogleTest 和 Benchmark 自身的附属测试、安装均关闭；项目只安装 CLI。
- 首次启用框架需要 Git 和访问 GitHub 的网络；依赖源码放在对应构建目录的 `_deps/` 中，不提交。
- 业务功能尚无持久化、并发、网络接口和淘汰策略，退出进程即丢失内存数据。

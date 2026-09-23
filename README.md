# kv_v0

内存级 KV 项目，使用 C++20 和 CMake。当前只包含工程配置和空程序入口，尚未实现 KV 业务。

## 环境

- 支持 C++20 的编译器，例如 GCC、Clang 或 MSVC。
- CMake >= 3.21。
- Ninja（仓库内 Presets 使用 Ninja）。
- clang-format 可选，用于代码格式化。

Windows 使用 MSVC 时，在初始化编译器环境的开发者终端内运行。

## 目录

```text
.
├── CMakeLists.txt          # 工程入口、测试开关
├── CMakePresets.json       # 共享 Debug / Release 配置
├── .clang-format          # C++ 格式规范
├── .editorconfig          # 编辑器通用配置
├── .github/workflows/ci.yml
├── libs/
│   └── core/
│       ├── CMakeLists.txt # KV::Core 静态库及公开编译要求
│       ├── include/kv/core/ # 公开头文件
│       ├── src/
│       │   └── core.cpp  # 空翻译单元，无业务实现
│       └── tests/
│           ├── CMakeLists.txt
│           └── build_smoke.cpp
├── apps/
│   └── cli/
│       ├── CMakeLists.txt # 链接核心库、安装程序
│       └── main.cpp      # 空程序入口
├── docs/
│   └── architecture.md   # 模块边界和构建约定
└── build/                 # 构建产物，不提交
```

## 构建和测试

```bash
cmake --preset debug
cmake --build --preset debug --parallel
ctest --preset debug --no-tests=error
./build/debug/apps/cli/kv_cli

cmake --preset release
cmake --build --preset release --parallel
ctest --preset release --no-tests=error
cmake --install build/release --prefix ./build/install
```

安装后的程序位于 `build/install/bin/`；Windows 可执行文件带 `.exe` 后缀。
目前程序直接返回成功，冒烟测试仅验证可执行程序能构建并启动，不验证任何 KV 行为。

关闭测试可使用 `cmake --preset release -DBUILD_TESTING=OFF`，重新开启时传 `ON`。
编译数据库生成到 `build/debug/compile_commands.json`，可供编辑器或 clangd 使用。
例如配置 clangd 参数 `--compile-commands-dir=build/debug`。
个人配置放在不提交的 `CMakeUserPresets.json`，不要在共享配置中写机器绝对路径。

## 后续业务代码的放置约定

- 对外 API 放 `libs/core/include/kv/core/`，使用 `#include <kv/core/store.hpp>` 这样的路径。
- 实现和私有头文件放 `libs/core/src/`；源文件在模块 CMake 中显式列出。
- 核心库 target 为 `kv_core`，通过别名 `KV::Core` 供 CLI 和测试链接。
- `core.cpp` 是构建占位文件，写入业务代码时替换为实际实现；模块测试放 `libs/core/tests/`。
- `main.cpp` 负责输入和程序组装，KV 读写逻辑放入库；测试直接调用库 API。
- 只为实际出现的职责建立子目录，不预建网络、持久化、分片或淘汰策略模块。
- 生成文件放构建目录；新增依赖时固定版本，通过 CMake target 声明依赖。

CI 在 Linux 上分别构建、测试并安装 Debug 和 Release；其他平台尚未由 CI 验证。

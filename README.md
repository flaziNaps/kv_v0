# kv_v0

一个用 C++20 编写的命令行键值存储程序。你可以把它当作一个临时的小字典：用名字（key）保存一段内容（value），再通过名字查找、修改或删除。

例如，输入 `set name alice` 保存名字，再输入 `get name` 就能看到 `alice`。

**数据只保存在内存中，退出程序后就会丢失。** 当前不需要配置数据库，也不需要启动网络服务。

## 1. 准备运行环境

第一次运行需要安装工具；以后可以直接进入项目目录，从编译或启动步骤开始。

本指南主要使用 Ubuntu 的终端命令，仓库的自动化构建也使用 Ubuntu 24.04。Windows 同学可以通过下面的 WSL 方式操作。

### Windows：先进入 Ubuntu

如果已经安装并能打开 WSL Ubuntu，可以跳过这一步。

1. 在开始菜单搜索 PowerShell，右键选择“以管理员身份运行”。
2. 执行：

   ```powershell
   wsl --install -d Ubuntu-24.04
   ```

3. 按系统提示完成安装，必要时重启电脑。
4. 从开始菜单打开 Ubuntu，按提示创建 Linux 用户名和密码。输入密码时屏幕不显示字符，这是正常的。

**后面的命令全部在 Ubuntu 窗口执行，不在 PowerShell 或 CMD 中执行。**

### Ubuntu：安装工具

打开终端（Ubuntu 桌面可以按 `Ctrl + Alt + T`），依次执行：

```bash
sudo apt update
sudo apt install -y git build-essential cmake ninja-build
```

`sudo` 可能要求输入当前 Linux 用户的密码。这里安装的工具分别用于下载代码、编译 C++、生成构建配置和执行构建。

检查是否安装成功：

```bash
git --version
g++ --version
cmake --version
ninja --version
```

每条命令都应显示版本号。项目要求 **CMake 3.21 或以上**、支持 **C++20** 的编译器，以及 Ninja。Ubuntu 24.04 仓库提供的这些工具可以用于本项目。

### macOS 或其他环境

macOS 需要先安装 Xcode 命令行工具（`xcode-select --install`）。如果已经安装 Homebrew，可以执行 `brew install cmake ninja`；之后使用下文相同的下载、编译和运行命令。

其他 Linux 发行版请用自己的包管理器安装 Git、C++20 编译器、CMake 和 Ninja。测试使用 GoogleTest，不依赖 Bash 脚本；Windows 新手仍建议使用上面的 WSL 路径。

## 2. 下载项目，进入项目目录

在终端中依次执行：

```bash
git clone https://github.com/Brucescan/kv_v0.git
cd kv_v0
```

如果已经下载了项目，直接用 `cd` 进入已有的项目文件夹，不必重复下载。

输入下面的命令检查位置：

```bash
ls
```

能看到 `README.md`、`CMakeLists.txt`、`CMakePresets.json`、`apps` 和 `libs`，说明位置正确。

**下文所有构建、启动和测试命令，都在这个目录执行。** 代码块中的命令可以复制，逐行执行；某一步报错时，先解决报错，再继续下一步。

## 3. 配置并编译

第一次运行时，依次执行：

```bash
cmake --preset debug
cmake --build --preset debug --parallel
```

- 第一条命令检查编译环境，并在 `build/debug` 中生成构建文件；看到 `Configuring done` 和 `Generating done` 表示配置完成。
- 第二条命令把源代码编译成可执行程序；执行结束且没有报错后，就可以启动。

这里的 `debug` 是仓库已经写好的构建配置名称，不需要自己创建或修改配置文件。首次配置会通过 Git 从 GitHub 下载 GoogleTest v1.17.0；Release 默认还会下载 Google Benchmark v1.9.4。请保持网络可用，无需全局安装这两个框架。

以后修改 C++ 源代码后，重新执行第二条编译命令即可。修改 CMake 配置或测试定义后，需要先重新执行第一条命令，再编译。

## 4. 启动并试用

执行：

```bash
./build/debug/apps/cli/kv_cli
```

出现下面的提示符，说明程序已经启动，正在等待输入：

```text
kvstore>
```

此时输入的是程序自己的命令。每输入一行，按一次回车。下面是一轮完整操作示例，**只输入 `kvstore>` 后面的内容，不要输入提示符本身**：

```text
kvstore> set name alice
set key sucess
kvstore> get name
alice
kvstore> contains name
 true
kvstore> showAll
Current data in the store:
name: alice
kvstore> set name bob
set key sucess
kvstore> get name
bob
kvstore> del name
del key name sucess
kvstore> get name
Key not found
kvstore> quit
```

输出中的 `sucess` 是当前程序实际使用的提示文字。

输入 `quit` 后会回到终端。下次启动仍使用 `./build/debug/apps/cli/kv_cli`，但上次保存的数据不会保留。

### 支持哪些命令？

| 命令 | 作用 | 示例 |
| --- | --- | --- |
| `set <key> <value>` | 保存内容；同名 key 已存在时覆盖原值 | `set name alice` |
| `get <key>` | 查询内容；不存在时显示 `Key not found` | `get name` |
| `del <key>` | 删除数据；提示删除成功或失败 | `del name` |
| `contains <key>` | 判断 key 是否存在，输出 `true` 或 `false` | `contains name` |
| `showAll` | 显示全部数据；为空时显示 `No data in the store.` | `showAll` |
| `quit` | 退出程序 | `quit` |

`<key>` 和 `<value>` 是要替换的内容，不要输入尖括号。命令区分大小写，例如 `showAll` 不能写成 `showall`。

当前输入按空白字符分隔，**key 和 value 请使用不带空格的单个词**，例如 `set city Beijing`。引号不能让含空格的内容变成一个参数；`set note hello world` 实际只会保存 `hello`，后面的内容会被忽略。`showAll` 的显示顺序不固定。

## 5. 运行自动化测试

先输入 `quit` 退出程序，再在项目目录执行：

```bash
ctest --preset debug
```

目前共有 24 个 GoogleTest 用例：7 个核心存储用例，以及 17 个 CLI 用例。CTest 会分别显示每个用例的名称，全部通过时显示 `100% tests passed`。

核心测试覆盖读写、覆盖、删除及存在性判断；CLI 测试向与主程序共用的命令循环输入命令，检查提示、参数校验、showAll、quit 和输入结束行为。CLI 测试在进程内执行，无需 Bash，不会等待键盘输入。

只运行核心测试或 CLI 测试：

```bash
ctest --preset debug -R '^MemoryStoreTest\.'
ctest --preset debug -R '^Commands/CliTest\.'
```

列出所有测试，或查看详细执行输出：

```bash
ctest --preset debug -N
ctest --preset debug -V
```

## 6. 常见问题

| 遇到的情况 | 怎么处理 |
| --- | --- |
| 提示 `command not found` | 根据缺少的命令，检查第 1 步的工具安装是否完成。Windows 用户确认自己在 Ubuntu 窗口中。 |
| 找不到 `CMakePresets.json` 或提示当前目录没有 `CMakeLists.txt` | 先用 `cd` 进入项目目录，再用 `ls` 检查位置。 |
| CMake 版本太低或编译器不支持 C++20 | 检查工具版本。若使用较旧的 Ubuntu，建议在 Ubuntu 24.04 环境按第 1 步准备工具。 |
| 启动时提示 `No such file or directory` | 确认在项目目录，并且第 3 步的编译已经成功；只有配置成功还不会生成程序。 |
| 出现 `kvstore>` 后一直不动 | 这是等待输入的正常状态，输入 `set name alice` 并按回车即可。 |
| 输入 `cmake`、`ctest` 等命令却提示 `Unknown command` | 你还在程序内部。先输入 `quit`，回到终端再执行这些命令。 |
| 提示 `Invalid number of arguments` | 对照命令表检查参数数量，例如 `get` 后需要一个 key，`set` 后需要 key 和 value。 |
| 重新启动后找不到之前的数据 | 当前是内存存储，退出后数据就会丢失。 |
| 仍显示旧的 `kv.cli.smoke` / `kv.cli.integration` 测试 | 重新执行 `cmake --preset debug`，再编译和测试，以更新旧的测试定义。 |
| GoogleTest / Benchmark 下载失败 | 检查本机 Git 能否通过 HTTPS 连接 GitHub，以及代理设置，然后重新配置；不要关闭 TLS 校验。 |

仍然失败时，请把**执行的命令和完整报错文本**一起发给维护者，方便定位问题。

## 7. 可选：Release 构建和安装

先完成上面的 Debug 流程即可使用项目。如果需要优化构建，可以执行：

```bash
cmake --preset release
cmake --build --preset release --parallel
ctest --preset release
```

启动 Release 程序：

```bash
./build/release/apps/cli/kv_cli
```

也可以将编译好的程序安装到项目内的目录，无需 `sudo`：

```bash
cmake --install build/release --prefix ./build/install
./build/install/bin/kv_cli
```

## 8. 运行性能测试（Google Benchmark）

第 7 步的 Release 配置默认启用性能测试。配置、编译完成后运行：

```bash
./build/release/libs/core/benchmarks/kv_core_bench
```

目前测量两类操作：`BM_GetExisting` 在不同数据量下读取已有 key，`BM_SetExisting` 使用不同长度的 value 覆盖已有 key。初始化数据和控制台输出不计入测量循环；读取包含返回字符串副本的成本，覆盖写包含实际字符串赋值的成本。

快速验证能否运行，或导出 JSON 结果：

```bash
./build/release/libs/core/benchmarks/kv_core_bench --benchmark_min_time=0.01s
./build/release/libs/core/benchmarks/kv_core_bench --benchmark_out=build/release/benchmark.json --benchmark_out_format=json
```

性能测试单独运行，不属于 `ctest`。用 Release 测性能，Debug 不适合作为正式性能结论；不同机器、CPU 负载和电源设置会影响结果，不设置固定耗时通过阈值。

两个框架有独立开关：`BUILD_TESTING` 控制 GoogleTest，`BUILD_BENCHMARKS` 控制 Google Benchmark。Debug 默认只启用 GoogleTest，Release 默认两者都启用。例如，只编译可执行程序、不下载任何测试框架：

```bash
cmake -S . -B build/minimal -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DBUILD_BENCHMARKS=OFF
cmake --build build/minimal --parallel
./build/minimal/apps/cli/kv_cli
```

如果不需要 Release 性能测试，可以使用 `cmake --preset release -DBUILD_BENCHMARKS=OFF`，之后再编译。

框架的接入方式参照 [GoogleTest 官方 CMake 指南](https://google.github.io/googletest/quickstart-cmake.html) 和 [Google Benchmark 用户指南](https://github.com/google/benchmark/blob/v1.9.4/docs/user_guide.md)。

## 9. 代码在哪里？

```text
kv_v0/
├── README.md                  # 使用说明
├── CMakeLists.txt             # 构建入口
├── CMakePresets.json          # Debug / Release 预设
├── apps/cli/
│   ├── main.cpp               # 程序入口
│   ├── cli.cpp                # 命令输入、解析和执行
│   ├── cli.hpp                # 命令循环声明
│   └── tests/cli_test.cpp     # GoogleTest CLI 测试
├── libs/core/
│   ├── include/kv/core/       # 存储接口和头文件
│   ├── src/memory_store.cpp   # 内存存储实现
│   ├── tests/store_test.cpp   # GoogleTest 核心功能测试
│   └── benchmarks/store_benchmark.cpp # Google Benchmark 性能测试
├── cmake/Dependencies.cmake   # 固定版本的测试框架依赖
├── docs/architecture.md       # 架构说明
└── build/                     # 配置、编译后生成的文件
```

运行项目只需要前面的步骤；想了解内部实现，再从 `apps/cli/cli.cpp` 和 `libs/core/src/memory_store.cpp` 开始阅读。

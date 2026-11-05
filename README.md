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
bash --version
```

每条命令都应显示版本号。项目要求 **CMake 3.21 或以上**、支持 **C++20** 的编译器，以及 Ninja。Ubuntu 24.04 仓库提供的这些工具可以用于本项目。

### macOS 或其他环境

macOS 需要先安装 Xcode 命令行工具（`xcode-select --install`）。如果已经安装 Homebrew，可以执行 `brew install cmake ninja`；之后使用下文相同的下载、编译和运行命令。

其他 Linux 发行版请用自己的包管理器安装 Git、C++20 编译器、CMake、Ninja 和 Bash。当前测试脚本依赖 Bash，Windows 新手建议使用上面的 WSL 路径。

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

这里的 `debug` 是仓库已经写好的构建配置名称，不需要自己创建或修改配置文件。项目不需要下载第三方 C++ 库。

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

正常情况下会运行三个测试：

| 测试名称 | 检查内容 |
| --- | --- |
| `kv.core.test` | 核心存储功能 |
| `kv.cli.smoke` | 程序能启动，并通过 `quit` 正常退出 |
| `kv.cli.integration` | 通过真实命令输入检查 CLI 行为 |

最后看到下面的信息，表示全部通过：

```text
100% tests passed, 0 tests failed out of 3
```

如果测试失败，终端会显示失败测试的输出。想只检查 CLI 冒烟测试并查看详细过程，可以运行：

```bash
ctest --preset debug -R '^kv.cli.smoke$' -V
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
| `kv.cli.smoke` 超时 | 确认使用了包含自动输入 `quit` 的测试配置，然后重新执行 `cmake --preset debug`、编译和测试。仅修改 CMake 文件不会自动更新已经生成的 CTest 定义。 |

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

## 8. 代码在哪里？

```text
kv_v0/
├── README.md                  # 使用说明
├── CMakeLists.txt             # 构建入口
├── CMakePresets.json          # Debug / Release 预设
├── apps/cli/
│   ├── main.cpp               # 命令输入、解析和执行
│   └── tests/cli_integration.sh # CLI 集成测试
├── libs/core/
│   ├── include/kv/core/       # 存储接口和头文件
│   ├── src/memory_store.cpp   # 内存存储实现
│   └── tests/store_test.cpp   # 核心功能测试
├── docs/architecture.md       # 架构说明
└── build/                     # 配置、编译后生成的文件
```

运行项目只需要前面的步骤；想了解内部实现，再从 `apps/cli/main.cpp` 和 `libs/core/src/memory_store.cpp` 开始阅读。

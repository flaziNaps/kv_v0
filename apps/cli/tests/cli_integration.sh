#!/usr/bin/env bash
# CLI 集成测试：跑真实的 kv_cli 可执行程序，验证 require.md 里"命令级"的行为
# （提示信息、参数数量校验、showAll 输出、quit 退出等）。
#
# 用法：bash cli_integration.sh <kv_cli 可执行文件路径>
# 全部通过返回 0，有任何失败返回 1（ctest 据此判红/绿）。

set -u

BIN="${1:?需要一个参数：kv_cli 可执行文件的路径}"

pass=0
fail=0

# run <描述> <期望出现的子串> <喂给程序的输入> [禁止出现的子串]
run() {
    local desc="$1" expect="$2" input="$3" forbidden="${4:-}"
    local out ok=1
    out=$(printf '%s' "$input" | "$BIN" 2>&1)

    grep -qF -- "$expect" <<<"$out" || ok=0
    if [[ -n "$forbidden" ]] && grep -qF -- "$forbidden" <<<"$out"; then
        ok=0
    fi

    if [[ $ok -eq 1 ]]; then
        echo "PASS: $desc"
        pass=$((pass + 1))
    else
        echo "FAIL: $desc"
        echo "    期望找到:   $expect"
        [[ -n "$forbidden" ]] && echo "    期望不出现: $forbidden"
        echo "    实际输出:"
        sed 's/^/      /' <<<"$out"
        fail=$((fail + 1))
    fi
}

# --- 需求 1 ---
run "set 后 get 得到原值"          "alice"              $'set name alice\nget name\nquit\n'
run "get 不存在的 key 提示未找到"  "Key not found"      $'get nope\nquit\n'
run "空输入不崩溃且有提示"          "No command provided" $'\nquit\n'
run "set 重复 key 覆盖旧值"        "bob"                $'set name alice\nset name bob\nget name\nquit\n' "alice"

# --- 需求 2 + 补充 7 ---
run "del 后 get 不到"              "Key not found"      $'set a 1\ndel a\nget a\nquit\n'
run "del 不存在的 key 提示失败"     "failed"             $'del nope\nquit\n'

# --- 需求 3 ---
run "参数数量不对时提示"           "Invalid number of arguments" $'set onlykey\nquit\n'

# --- 需求 4 + 补充 11 ---
run "showAll 展示数据"             "a: 1"               $'set a 1\nshowAll\nquit\n'
run "showAll 空存储提示"           "No data in the store." $'showAll\nquit\n'

# --- 需求 5 + 补充 12 ---
run "quit 后不再处理后续命令"      "kvstore>"           $'quit\nset x y\n' "set key sucess"

# --- 需求 6 + 补充 10 ---
run "contains 存在的 key 为 true"  " true"              $'set a 1\ncontains a\nquit\n'
run "contains 不存在的 key 为 false" " false"            $'contains nope\nquit\n'

# --- 补充 9 ---
run "未知命令提示"                 "Unknown command"    $'foo\nquit\n'

echo
echo "通过 $pass 个，失败 $fail 个"
[[ $fail -eq 0 ]]

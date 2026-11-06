#include <benchmark/benchmark.h>
#include <kv/core/memory_store.hpp>
#include <string>

static void BM_GetExisting(benchmark::State& state)
{
    kv::MemoryStore store;
    for (int i = 0; i < state.range(0); ++i) {
        store.set(std::to_string(i), "value");
    }
    const std::string key = std::to_string(state.range(0) / 2);
    for (auto _ : state) {
        auto value = store.get(key);
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_GetExisting)->Arg(100)->Arg(10000);

static void BM_SetExisting(benchmark::State& state)
{
    kv::MemoryStore store;
    const std::string value(state.range(0), 'x');
    store.set("key", value);
    for (auto _ : state) {
        store.set("key", value);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SetExisting)->Arg(16)->Arg(1024);

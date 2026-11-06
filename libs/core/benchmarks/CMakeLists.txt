add_executable(kv_core_bench store_benchmark.cpp)
target_link_libraries(kv_core_bench PRIVATE KV::Core benchmark::benchmark_main)

add_executable(kv_core_test store_test.cpp)
target_link_libraries(kv_core_test PRIVATE KV::Core)

# store_test.cpp 里是对 MemoryStore 的行为断言；断言失败会以非零退出码报错。
add_test(NAME kv.core.test COMMAND kv_core_test)
set_tests_properties(kv.core.test PROPERTIES TIMEOUT 5)

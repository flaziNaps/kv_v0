add_executable(kv_core_test store_test.cpp)
target_link_libraries(kv_core_test PRIVATE KV::Core GTest::gtest_main)
gtest_discover_tests(kv_core_test DISCOVERY_TIMEOUT 10 PROPERTIES TIMEOUT 5)

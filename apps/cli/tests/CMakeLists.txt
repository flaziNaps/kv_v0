add_executable(kv_cli_test cli_test.cpp)
target_link_libraries(kv_cli_test PRIVATE kv_cli_commands GTest::gtest_main)
gtest_discover_tests(kv_cli_test NO_PRETTY_VALUES DISCOVERY_TIMEOUT 10 PROPERTIES TIMEOUT 5)

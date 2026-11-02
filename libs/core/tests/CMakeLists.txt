add_executable(kv_core_build_smoke store_test.cpp)
target_link_libraries(kv_core_build_smoke PRIVATE KV::Core)

# Checks build plumbing only; add behavioral tests with the KV implementation.
add_test(NAME kv.core.build_smoke COMMAND kv_core_build_smoke)
set_tests_properties(kv.core.build_smoke PROPERTIES TIMEOUT 5)

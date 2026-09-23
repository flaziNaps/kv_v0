add_executable(kv_cli main.cpp)
target_link_libraries(kv_cli PRIVATE KV::Core)

if(MSVC)
    target_compile_options(kv_cli PRIVATE /W4 /permissive-)
else()
    target_compile_options(kv_cli PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_test(NAME kv.cli.smoke COMMAND kv_cli)
    set_tests_properties(kv.cli.smoke PROPERTIES TIMEOUT 5)
endif()

install(TARGETS kv_cli RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

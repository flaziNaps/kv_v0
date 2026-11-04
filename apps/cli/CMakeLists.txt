add_executable(kv_cli main.cpp)
target_link_libraries(kv_cli PRIVATE KV::Core)

if(MSVC)
    target_compile_options(kv_cli PRIVATE /W4 /permissive-)
else()
    target_compile_options(kv_cli PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_test(NAME kv.cli.smoke
             COMMAND bash -c "echo quit | \"$<TARGET_FILE:kv_cli>\"")
    set_tests_properties(kv.cli.smoke PROPERTIES TIMEOUT 5)

    add_test(NAME kv.cli.integration
             COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_integration.sh $<TARGET_FILE:kv_cli>)
    set_tests_properties(kv.cli.integration PROPERTIES TIMEOUT 10)
endif()

install(TARGETS kv_cli RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

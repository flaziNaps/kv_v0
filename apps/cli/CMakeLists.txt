add_library(kv_cli_commands STATIC cli.cpp cli.hpp)
target_link_libraries(kv_cli_commands PUBLIC KV::Core)
target_include_directories(kv_cli_commands PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(kv_cli main.cpp)
target_link_libraries(kv_cli PRIVATE kv_cli_commands)

if(MSVC)
    target_compile_options(kv_cli_commands PRIVATE /W4 /permissive-)
else()
    target_compile_options(kv_cli_commands PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

install(TARGETS kv_cli RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

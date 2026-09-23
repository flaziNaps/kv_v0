add_library(kv_core STATIC
    src/core.cpp
)
add_library(KV::Core ALIAS kv_core)

target_compile_features(kv_core PUBLIC cxx_std_20)
target_include_directories(kv_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
    target_compile_options(kv_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(kv_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

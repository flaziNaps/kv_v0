cmake_minimum_required(VERSION 3.21)

project(kv VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(GNUInstallDirs)
option(BUILD_BENCHMARKS "Build KV benchmarks" OFF)
include(cmake/Dependencies.cmake)

add_subdirectory(libs/core)
add_subdirectory(apps/cli)

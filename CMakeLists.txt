cmake_minimum_required(VERSION 3.25.1)

project(seqlib VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(seqlib STATIC src/vec.c src/que.c)
add_library(algo STATIC src/algo/search.c)

add_subdirectory(lib/cmth)

target_include_directories(algo PRIVATE include lib)
target_include_directories(seqlib PRIVATE include lib)
target_link_libraries(seqlib algo cmth)

enable_testing()
add_subdirectory(test)
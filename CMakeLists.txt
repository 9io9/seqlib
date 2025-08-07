cmake_minimum_required(VERSION 3.25.1)

project(seqlib VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/ec.h 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/pec/pec.py ${CMAKE_CURRENT_SOURCE_DIR}/.ec ${CMAKE_CURRENT_SOURCE_DIR}/include/ec.h _LJW_SEQLIB_EC_H_
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/.ec ${CMAKE_CURRENT_SOURCE_DIR}/tools/pec/pec.py
    COMMENT "convert .ec to error code header file"
)

add_custom_target(seqgech DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/ec.h)

add_library(seqlib STATIC src/vec.c src/que.c src/list.c src/arr.c)
add_library(algo STATIC src/algo/search.c)
add_dependencies(seqlib seqgech)

add_subdirectory(lib/cmth)
add_subdirectory(lib/aLocas)

target_include_directories(algo PRIVATE include lib)
target_include_directories(seqlib PRIVATE include lib)
target_link_libraries(seqlib algo cmth aLocas)

enable_testing()
add_subdirectory(test)
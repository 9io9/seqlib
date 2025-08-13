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

target_include_directories(seqlib PRIVATE include lib)
target_include_directories(algo PRIVATE include lib)

find_package(aLocas REQUIRED)
find_package(cmth REQUIRED)

target_link_libraries(seqlib aLocas::aLocas cmth::cmth algo)

set_target_properties(seqlib PROPERTIES EXPORT_NAME seqlib)

install(
    TARGETS seqlib algo
    EXPORT seqlibTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include DESTINATION .)
install(
    EXPORT seqlibTargets
    FILE seqlibTargets.cmake
    NAMESPACE seqlib::
    DESTINATION lib/cmake/seqlib
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    seqlibConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(seqlibConfig.cmake.in seqlibConfig.cmake @ONLY)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/seqlibConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/seqlibConfigVersion.cmake"
    DESTINATION lib/cmake/seqlib
)
install(
    EXPORT seqlibTargets
    FILE seqlibTargets.cmake
    NAMESPACE seqlib::
    DESTINATION lib/cmake/seqlib
)

enable_testing()
add_subdirectory(test)
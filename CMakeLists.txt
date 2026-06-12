cmake_minimum_required(VERSION 3.20)
project(Vector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Vector INTERFACE
        include/vector.hpp
)

target_include_directories(Vector INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(Vector INTERFACE cxx_std_20)

install(DIRECTORY include/
    DESTINATION include
)

install(TARGETS Vector
    EXPORT VectorTargets
)

install(EXPORT VectorTargets
    FILE VectorTargets.cmake
    NAMESPACE Vector::
    DESTINATION lib/cmake/Vector
)

include(CMakePackageConfigHelpers)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake.in
    "@PACKAGE_INIT@\n"
    "include(\"\${CMAKE_CURRENT_LIST_DIR}/VectorTargets.cmake\")\n"
    "check_required_components(Vector)\n"
)

configure_package_config_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake
    INSTALL_DESTINATION lib/cmake/Vector
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfigVersion.cmake
    DESTINATION lib/cmake/Vector
)

export(EXPORT VectorTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/VectorTargets.cmake
    NAMESPACE Vector::
)

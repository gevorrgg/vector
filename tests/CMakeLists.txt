cmake_minimum_required(VERSION 3.20)
project(vector_tests)

set(CMAKE_CXX_STANDARD 20)
include(CTest)

find_program(MEMORYCHECK_COMMAND valgrind)

if(MEMORYCHECK_COMMAND)
    set(MEMORYCHECK_COMMAND_OPTIONS
        "--leak-check=full
         --show-leak-kinds=all
         --track-origins=yes
         --error-exitcode=1
         --quiet"
    )

    set(MEMORYCHECK_SUPPRESSIONS_FILE "")
endif()

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)

FetchContent_MakeAvailable(googletest)

find_package(Vector CONFIG REQUIRED)

add_library(test_support INTERFACE)

target_include_directories(test_support INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test_support INTERFACE
        GTest::gtest
        GTest::gtest_main
)

add_subdirectory(unit_tests)
add_subdirectory(exception_safety_tests)

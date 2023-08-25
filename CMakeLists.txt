cmake_minimum_required(VERSION 3.1)

project(ICG VERSION 2.0
            DESCRIPTION "Interface Code Generator (ICG) 2: Electric Boogaloo"
            LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set(CMAKE_CXX_FLAGS "-fsanitize=address" CACHE STRING "Asan!" FORCE)

option(ICG_BUILD_TESTS "Build the tests" ON)
if(ICG_BUILD_TESTS)
    message("Testing is enabled.")
    set (TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/test_results)

    # Add a target check for convenience
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

    enable_testing()
    add_subdirectory(test)

    # Tell DataTypes to turn on its tests as well
    option(DECL_BUILD_TESTS "Build the LibDecl tests" ON)
endif()


option(ICG_ENABLE_COVERAGE "Enable code coverage generation" OFF)
if (ICG_ENABLE_COVERAGE)
    message("Coverage is enabled.")

    set(CMAKE_C_FLAGS_DEBUG "-O0 -g -fprofile-arcs -ftest-coverage" CACHE STRING "Extra compile flags required by code coverage" FORCE)
    set(CMAKE_CXX_FLAGS "-O0 -g -fprofile-arcs -ftest-coverage" CACHE STRING "Extra compile flags required by code coverage" FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage" CACHE STRING "Extra linker flags required by code coverage" FORCE)

    add_custom_target(coverage 
                    DEPENDS coverage.lcov
                    )

    add_custom_command(
                    OUTPUT full_coverage.lcov
                    DEPENDS check
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMAND lcov -q -c -d . -o full_coverage.lcov
                    COMMENT "========================== Gathering coverage information... =========================="
                    )

    add_custom_command(
                    OUTPUT coverage.lcov
                    DEPENDS full_coverage.lcov check
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMAND lcov -q -r full_coverage.lcov 'Library/*' '/usr/include/*' '*/extern/*' '*/test/*' -o coverage.lcov
                    COMMENT "========================== Filtering coverage information... =========================="
                    )
endif()


add_subdirectory(extern)
add_subdirectory(ICG)
add_subdirectory(DataTypes)




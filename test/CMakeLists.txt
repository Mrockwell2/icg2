cmake_minimum_required(VERSION 3.1)

macro(add_icg_integration_test DIRNAME HEADERNAME)

    # The test name is the same as the dirname
    set ( TESTNAME
        ${DIRNAME}
    )

    string(CONCAT ICG_COMMENT  "============================= ICGing " ${HEADERNAME} "... =============================" )

    # Generate the io info
    add_custom_command(
        OUTPUT ${DIRNAME}/io_${HEADERNAME}
        DEPENDS icg
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test/${DIRNAME}
        COMMAND ${CMAKE_SOURCE_DIR}/build/bin/icg ${HEADERNAME}
        COMMENT ${ICG_COMMENT}
    )

    # Create an executable
    add_executable(${TESTNAME} ${DIRNAME}/test_main.cpp ${DIRNAME}/io_${HEADERNAME})

    # Include test utilities
    target_include_directories(${TESTNAME} PUBLIC ${CMAKE_SOURCE_DIR}/test/common)

    # Link in gtest and the decl library
    target_link_libraries(${TESTNAME} gtest gtest_main decl)

    # Tell cmake this test exists and how to run it
    add_test(NAME ${TESTNAME} 
             COMMAND python3 ${CMAKE_SOURCE_DIR}/test/test_driver.py ${CMAKE_SOURCE_DIR}/test/${DIRNAME} ${CMAKE_BINARY_DIR}/bin/${TESTNAME} --gtest_output=xml:${TEST_OUTPUT_DIR}/${TESTNAME}.xml)  

    add_dependencies(check ${TESTNAME})
endmacro()

add_icg_integration_test(basic_checkpoint_restore header.hpp)
add_icg_integration_test(partial_checkpoint header.hpp)
add_icg_integration_test(namespaces bar.hpp)
add_icg_integration_test(nested_classes bar.hpp)


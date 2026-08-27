option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    )

    if(DEFINED TEST_SOURCES AND NOT STREQUAL "")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "tests")
        foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
            get_filename_component(TEST_NAME
                ${TEST_SOURCE}
                NAME_WE
            )
            add_executable(${TEST_NAME}
                ${TEST_SOURCE}
            )
            target_link_libraries(${TEST_NAME}
                PRIVATE
                    SmL
            )
            add_test(
                NAME ${TEST_NAME}
                COMMAND ${TEST_NAME}
            )
        endforeach()
    endif()
endif()

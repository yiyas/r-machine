
file(GLOB test_srcs *.c)

set(TEST_DEP_LIBS pthread)
if(ENABLE_GCOV)
    set(TEST_DEP_LIBS gcov)
endif(ENABLE_GCOV)

foreach(test_src IN LISTS test_srcs)
    string(REGEX REPLACE "(.*/)?(.*)\\.c$" "\\2" test_name ${test_src})
    string(FIND ${test_name} "test_" test_pos)
    if(${test_pos} EQUAL 0)
        list(APPEND tests ${test_name})
        add_executable(${test_name} ${test_src} unity/unity.c)
        target_link_libraries(${test_name} ${TEST_DEP_LIBS} rmachine)
        add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}> WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
endforeach(test_src)

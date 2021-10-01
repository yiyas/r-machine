
file(GLOB_RECURSE test_srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_*.c)

set(TEST_DEP_LIBS pthread)
if(ENABLE_GCOV)
    set(TEST_DEP_LIBS gcov)
endif(ENABLE_GCOV)

foreach(test_src IN LISTS test_srcs)
    message(${test_src})
    
    string(REGEX REPLACE "(.+)\\.c$" "\\1" test_name ${test_src})
    string(REPLACE "/" "." test_name ${test_name})
    
    list(APPEND all_tests ${test_name})
    add_executable(${test_name} ${test_src} unity/unity.c)
    target_link_libraries(${test_name} ${TEST_DEP_LIBS} $<TARGET_OBJECTS:r_objs>)
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}> WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach(test_src)

find_program(VALGRIND_FOUND valgrind)
if(ENABLE_VALGRIND_TESTS)
    if(VALGRIND_FOUND)
        foreach(test_name IN LISTS all_tests)
            add_test(${test_name}_valgrind valgrind 
                --error-exitcode=1 
                --leak-check=full 
                --show-leak-kinds=all 
                --errors-for-leak-kinds=all
                ${CMAKE_BINARY_DIR}/tests/${test_name})
        endforeach(test_name)
    else(VALGRIND_FOUND)
        message(WARNING "valgrind executable not found! Disabling memory leaks tests.")
    endif(VALGRIND_FOUND)
endif(ENABLE_VALGRIND_TESTS)

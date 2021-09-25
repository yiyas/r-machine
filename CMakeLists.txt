cmake_minimum_required(VERSION 3.5)

project(r-machine C)

set(R_MAJOR_VERSION 0)
set(R_MINOR_VERSION 0)
set(R_MICRO_VERSION 1)
set(R_VERSION ${R_MAJOR_VERSION}.${R_MINOR_VERSION}.${R_MICRO_VERSION})

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -Wall -Wextra -D__FILENAME__='\"$(subst $(dir $<),,$<)\"'")
set(CMAKE_C_FLAGS_RELEASE   "-O2")
set(CMAKE_C_FLAGS_DEBUG     "-g -O0")

option(LIBAL_DIR "Libal directory.")

if(NOT LIBAL_DIR)
    set(LIBAL_DIR ${CMAKE_SOURCE_DIR}/../libal)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include  ${LIBAL_DIR}/include)

file(GLOB r_srcs src/*.c src/parser/*.c ${LIBAL_DIR}/src/*.c)

add_library(r_objs OBJECT ${r_srcs})

add_executable(rmachine $<TARGET_OBJECTS:r_objs> src/exe/main.c)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    option(ENABLE_TESTS "Build tests" ON)
    option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" ON)
endif()

find_package(FLEX)
find_package(BISON)

if(NOT BISON_FOUND)
    message(WARNING "Missing Bison.\nYou won't be able to generate source codes from changed flex/bison files.\nCompiling should still works fine.")
elseif (NOT FLEX_FOUND)
    message(WARNING "Missing Flex.\nYou won't be able to generate source codes from changed flex/bison files.\nCompiling should still works fine.")
else()
    add_custom_target(bison_flex
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser
        COMMAND bison -l -o parser_bison.c --defines=parser_bison.h parser.y
        COMMAND flex -8 -L -o parser_flex.c --header-file=parser_flex.h parser.l
    )
endif()

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_TESTS)

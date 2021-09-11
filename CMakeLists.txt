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
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE   "-O2")
set(CMAKE_C_FLAGS_DEBUG     "-g -O0")

option(LIBAL_DIR "Libal directory.")

if(NOT LIBAL_DIR)
    set(LIBAL_DIR ${CMAKE_SOURCE_DIR}/../libal)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include  ${LIBAL_DIR}/include)

file(GLOB r_srcs src/*.c)

add_library(r_objs OBJECT ${r_srcs})

add_executable(rmachine $<TARGET_OBJECTS:r_objs> src/exe/main.c)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    option(ENABLE_TESTS "Build tests" ON)
    option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" ON)
endif()

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_TESTS)

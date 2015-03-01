cmake_minimum_required(VERSION 2.6)
project(tcframe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

enable_testing()

include_directories(include)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES
    include/tcframe/constraint.h
    include/tcframe/exception.h
    include/tcframe/failure.h
    include/tcframe/generator.h
    include/tcframe/io.h
    include/tcframe/logger.h
    include/tcframe/os.h
    include/tcframe/problem.h
    include/tcframe/tcframe.h
    include/tcframe/testcase.h
    include/tcframe/type.h
    include/tcframe/util.h
    test/constraint_test.cpp
    test/exception_test.cpp
    test/failure_test.cpp
    test/io_test.cpp
    test/tcframe_test.cpp
    test/testcase_test.cpp
    test/type_test.cpp
    test/util_test.cpp)

add_executable(main_test ${SOURCE_FILES})
target_link_libraries(main_test ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

GTEST_ADD_TESTS(main_test "" ${SOURCE_FILES})

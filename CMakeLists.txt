cmake_minimum_required(VERSION 2.6)
project(tcframe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
set(CMAKE_CXX_COMPILER g++)

enable_testing()

include_directories(include)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES
    include/tcframe/constraint.hpp
    include/tcframe/exception.hpp
    include/tcframe/failure.hpp
    include/tcframe/generator.hpp
    include/tcframe/io.hpp
    include/tcframe/logger.hpp
    include/tcframe/macro.hpp
    include/tcframe/os.hpp
    include/tcframe/problem.hpp
    include/tcframe/runner.hpp
    include/tcframe/submitter.hpp
    include/tcframe/testcase.hpp
    include/tcframe/type.hpp
    include/tcframe/util.hpp
    include/tcframe/verdict.hpp
    test/constraint_test.cpp
    test/exception_test.cpp
    test/failure_test.cpp
    test/generator_test.cpp
    test/io_test.cpp
    test/macro_test.cpp
    test/problem_test.cpp
    test/submitter_test.cpp
    test/tcframe_test_commons.cpp
    test/testcase_test.cpp
    test/type_test.cpp
    test/util_test.cpp)

add_executable(all_tests ${SOURCE_FILES})
target_link_libraries(all_tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} gcov)

GTEST_ADD_TESTS(all_tests "" ${SOURCE_FILES})

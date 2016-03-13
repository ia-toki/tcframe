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
    include/tcframe/core/Failure.hpp
    include/tcframe/core/NotImplementedException.hpp
    include/tcframe/core/Util.hpp
    include/tcframe/core/Verdict.hpp
    include/tcframe/generator/BaseGenerator.hpp
    include/tcframe/generator/OfficialTestCase.hpp
    include/tcframe/generator/SampleTestCase.hpp
    include/tcframe/generator/TestCase.hpp
    include/tcframe/generator/TestCaseException.hpp
    include/tcframe/generator/TestCasesCollector.hpp
    include/tcframe/generator/TestGroup.hpp
    include/tcframe/ioformat/GridIOSegment.hpp
    include/tcframe/ioformat/IOFormatException.hpp
    include/tcframe/ioformat/IOFormatProvider.hpp
    include/tcframe/ioformat/IOSegment.hpp
    include/tcframe/ioformat/LineIOSegment.hpp
    include/tcframe/ioformat/LinesIOSegment.hpp
    include/tcframe/ioformat/MatrixSizes.hpp
    include/tcframe/ioformat/VectorSize.hpp
    include/tcframe/ioformat/VectorWithSize.hpp
    include/tcframe/iovariable/HorizontalVariable.hpp
    include/tcframe/iovariable/HorizontalVector.hpp
    include/tcframe/iovariable/Matrix.hpp
    include/tcframe/iovariable/MatrixVariable.hpp
    include/tcframe/iovariable/ParsingException.hpp
    include/tcframe/iovariable/PrintingException.hpp
    include/tcframe/iovariable/Scalar.hpp
    include/tcframe/iovariable/ScalarHorizontalVariable.hpp
    include/tcframe/iovariable/Variable.hpp
    include/tcframe/iovariable/VectorHorizontalVariable.hpp
    include/tcframe/iovariable/VerticalVariable.hpp
    include/tcframe/iovariable/VerticalVector.hpp
    include/tcframe/logger/DefaultGeneratorLogger.hpp
    include/tcframe/logger/DefaultLoggerUtils.hpp
    include/tcframe/logger/DefaultSubmitterLogger.hpp
    include/tcframe/logger/GeneratorLogger.hpp
    include/tcframe/logger/Logger.hpp
    include/tcframe/logger/SubmitterLogger.hpp
    include/tcframe/os/ExecutionException.hpp
    include/tcframe/os/ExecutionResult.hpp
    include/tcframe/os/OperatingSystem.hpp
    include/tcframe/os/UnixOperatingSystem.hpp
    include/tcframe/problem/BaseProblem.hpp
    include/tcframe/problem/Constraint.hpp
    include/tcframe/problem/ConstraintsCollector.hpp
    include/tcframe/problem/MultipleTestCasesConstraintsSatisfiabilityException.hpp
    include/tcframe/problem/Subtask.hpp
    include/tcframe/problem/SubtaskSatisfiabilityException.hpp
    include/tcframe/random/Random.hpp
    include/tcframe/runner/Macro.hpp
    include/tcframe/runner/Runner.hpp
    include/tcframe/submitter/Submitter.hpp
    include/tcframe/runner.hpp
    test/tcframe/commons/Commons.hpp
    test/tcframe/core/FailureTests.cpp
    test/tcframe/core/UtilTests.cpp
    test/tcframe/generator/BaseGeneratorTests.cpp
    test/tcframe/generator/OfficialTestCaseTests.cpp
    test/tcframe/generator/SampleTestCaseTests.cpp
    test/tcframe/generator/TestCasesCollectorTests.cpp
    test/tcframe/ioformat/GridIOSegmentTests.cpp
    test/tcframe/ioformat/IOFormatExceptionTests.cpp
    test/tcframe/ioformat/IOFormatProviderTests.cpp
    test/tcframe/ioformat/LineIOSegmentTests.cpp
    test/tcframe/ioformat/LinesIOSegmentTests.cpp
    test/tcframe/ioformat/ParsingExceptionTests.cpp
    test/tcframe/ioformat/PrintingExceptionTests.cpp
    test/tcframe/iovariable/HorizontalVectorTests.cpp
    test/tcframe/iovariable/MatrixTests.cpp
    test/tcframe/iovariable/ScalarTests.cpp
    test/tcframe/iovariable/VerticalVectorTests.cpp
    test/tcframe/os/ExecutionExceptionTests.cpp
    test/tcframe/problem/BaseProblemTests.cpp
    test/tcframe/problem/ConstraintsCollectorTests.cpp
    test/tcframe/problem/ConstraintTests.cpp
    test/tcframe/random/RandomTests.cpp
    test/tcframe/runner/MacroTests.cpp
    test/tcframe/submitter/SubmitterTests.cpp
)

add_executable(all_tests ${SOURCE_FILES})
target_link_libraries(all_tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} gcov)

GTEST_ADD_TESTS(all_tests "" ${SOURCE_FILES})

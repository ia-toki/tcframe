cmake_minimum_required(VERSION 2.6)
project(tcframe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

find_package(Git REQUIRED)
find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG d225acc90bc3a8c420a9bcd1f033033c1ccd7fe0
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
    CMAKE_ARGS ${CMAKE_ARGS} -DBUILD_GTEST=ON -DBUILD_GMOCK=ON -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(googletest source_dir binary_dir)

add_library(gtest IMPORTED STATIC GLOBAL)
add_library(gmock IMPORTED STATIC GLOBAL)

set_target_properties(
    gtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
set_target_properties(
    gmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock_main.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_dependencies(gtest googletest)
add_dependencies(gmock googletest)

include_directories("${source_dir}/googletest/include")
include_directories("${source_dir}/googlemock/include")

include_directories(include)

set(SOURCE_FILES
    include/tcframe/config.hpp
    include/tcframe/config/GeneratorConfig.hpp
    include/tcframe/config/ProblemConfig.hpp
    include/tcframe/constraint.hpp
    include/tcframe/constraint/Constraint.hpp
    include/tcframe/constraint/ConstraintGroup.hpp
    include/tcframe/constraint/ConstraintSuite.hpp
    include/tcframe/format.hpp
    include/tcframe/format/IOFormat.hpp
    include/tcframe/format/IOSegment.hpp
    include/tcframe/format/LineIOSegment.hpp
    include/tcframe/format/LineIOSegmentPrinter.hpp
    include/tcframe/format/LineIOSegmentVariable.hpp
    include/tcframe/format/LineIOSegmentScalarVariable.hpp
    include/tcframe/format/WhitespacePrinter.hpp
    include/tcframe/generation.hpp
    include/tcframe/generation/TestSuiteGenerationListener.hpp
    include/tcframe/generation/TestCaseData.hpp
    include/tcframe/generation/TestCaseNameCreator.hpp
    include/tcframe/generation/TestCaseGenerator.hpp
    include/tcframe/generation/TestCaseGenerationException.hpp
    include/tcframe/generation/TestCaseGenerationFailure.hpp
    include/tcframe/generation/TestCaseGenerationResult.hpp
    include/tcframe/generation/TestSuiteGenerator.hpp
    include/tcframe/generation/TestSuiteGenerationResult.hpp
    include/tcframe/generation/VerificationFailure.hpp
    include/tcframe/os.hpp
    include/tcframe/os/ExecutionResult.hpp
    include/tcframe/os/OperatingSystem.hpp
    include/tcframe/os/UnixOperatingSystem.hpp
    include/tcframe/experimental/runner.hpp
    include/tcframe/runner/BaseProblem.hpp
    include/tcframe/runner/BaseGenerator.hpp
    include/tcframe/runner/Magic.hpp
    include/tcframe/runner/Runner.hpp
    include/tcframe/testcase.hpp
    include/tcframe/testcase/TestCase.hpp
    include/tcframe/testcase/TestSuite.hpp
    include/tcframe/testcase/TestGroup.hpp
    include/tcframe/type.hpp
    include/tcframe/type/Scalar.hpp
    include/tcframe/util.hpp
    include/tcframe/util/StringUtils.hpp
    include/tcframe/variable.hpp
    include/tcframe/variable/IOVariablesPrinter.hpp
    include/tcframe/verification.hpp
    include/tcframe/verification/ConstraintSuiteVerifier.hpp
    include/tcframe/verification/ConstraintSuiteVerificationResult.hpp
    test/tcframe/constraint/ConstraintSuiteBuilderTests.cpp
    test/tcframe/format/FakeIOSegment.hpp
    test/tcframe/format/IOFormatBuilderTests.cpp
    test/tcframe/format/LineIOSegmentBuilderTests.cpp
    test/tcframe/format/LineIOSegmentPrinterTests.cpp
    test/tcframe/format/MockLineIOSegmentPrinter.hpp
    test/tcframe/format/MockWhitespacePrinter.hpp
    test/tcframe/testcase/TestSuiteBuilderTests.cpp
    test/tcframe/type/MockScalar.hpp
    test/tcframe/type/ScalarTests.cpp
    test/tcframe/variable/IOVariablePrinterTests.cpp
    test/tcframe/verification/ConstraintSuiteVerifierTests.cpp

    include/tcframe/deprecated/core/Failure.hpp
    include/tcframe/deprecated/core/NotImplementedException.hpp
    include/tcframe/deprecated/core/Util.hpp
    include/tcframe/deprecated/core/Verdict.hpp
    include/tcframe/deprecated/generator/BaseGenerator.hpp
    include/tcframe/deprecated/generator/OfficialTestCase.hpp
    include/tcframe/deprecated/generator/SampleTestCase.hpp
    include/tcframe/deprecated/generator/TestCase.hpp
    include/tcframe/deprecated/generator/TestCaseException.hpp
    include/tcframe/deprecated/generator/TestCasesCollector.hpp
    include/tcframe/deprecated/generator/TestGroup.hpp
    include/tcframe/deprecated/ioformat/GridIOSegment.hpp
    include/tcframe/deprecated/ioformat/IOFormatException.hpp
    include/tcframe/deprecated/ioformat/IOFormatProvider.hpp
    include/tcframe/deprecated/ioformat/IOSegment.hpp
    include/tcframe/deprecated/ioformat/LineIOSegment.hpp
    include/tcframe/deprecated/ioformat/LinesIOSegment.hpp
    include/tcframe/deprecated/ioformat/MatrixSizes.hpp
    include/tcframe/deprecated/ioformat/VectorSize.hpp
    include/tcframe/deprecated/ioformat/VectorWithSize.hpp
    include/tcframe/deprecated/iovariable/HorizontalVariable.hpp
    include/tcframe/deprecated/iovariable/HorizontalVector.hpp
    include/tcframe/deprecated/iovariable/Matrix.hpp
    include/tcframe/deprecated/iovariable/MatrixVariable.hpp
    include/tcframe/deprecated/iovariable/ParsingException.hpp
    include/tcframe/deprecated/iovariable/PrintingException.hpp
    include/tcframe/deprecated/iovariable/Scalar.hpp
    include/tcframe/deprecated/iovariable/ScalarHorizontalVariable.hpp
    include/tcframe/deprecated/iovariable/Variable.hpp
    include/tcframe/deprecated/iovariable/VectorHorizontalVariable.hpp
    include/tcframe/deprecated/iovariable/VerticalVariable.hpp
    include/tcframe/deprecated/iovariable/VerticalVector.hpp
    include/tcframe/deprecated/logger/DefaultGeneratorLogger.hpp
    include/tcframe/deprecated/logger/DefaultLoggerUtils.hpp
    include/tcframe/deprecated/logger/DefaultSubmitterLogger.hpp
    include/tcframe/deprecated/logger/GeneratorLogger.hpp
    include/tcframe/deprecated/logger/Logger.hpp
    include/tcframe/deprecated/logger/SubmitterLogger.hpp
    include/tcframe/deprecated/os/ExecutionException.hpp
    include/tcframe/deprecated/os/ExecutionResult.hpp
    include/tcframe/deprecated/os/OperatingSystem.hpp
    include/tcframe/deprecated/os/UnixOperatingSystem.hpp
    include/tcframe/deprecated/problem/BaseProblem.hpp
    include/tcframe/deprecated/problem/Constraint.hpp
    include/tcframe/deprecated/problem/ConstraintsCollector.hpp
    include/tcframe/deprecated/problem/MultipleTestCasesConstraintsSatisfiabilityException.hpp
    include/tcframe/deprecated/problem/Subtask.hpp
    include/tcframe/deprecated/problem/SubtaskSatisfiabilityException.hpp
    include/tcframe/deprecated/random/Random.hpp
    include/tcframe/deprecated/runner/Macro.hpp
    include/tcframe/deprecated/runner/Runner.hpp
    include/tcframe/deprecated/submitter/Submitter.hpp
    include/tcframe/runner.hpp
    test/tcframe/deprecated/commons/Commons.hpp
    test/tcframe/deprecated/core/FailureTests.cpp
    test/tcframe/deprecated/core/UtilTests.cpp
    test/tcframe/deprecated/generator/BaseGeneratorTests.cpp
    test/tcframe/deprecated/generator/OfficialTestCaseTests.cpp
    test/tcframe/deprecated/generator/SampleTestCaseTests.cpp
    test/tcframe/deprecated/generator/TestCasesCollectorTests.cpp
    test/tcframe/deprecated/ioformat/GridIOSegmentTests.cpp
    test/tcframe/deprecated/ioformat/IOFormatExceptionTests.cpp
    test/tcframe/deprecated/ioformat/IOFormatProviderTests.cpp
    test/tcframe/deprecated/ioformat/LineIOSegmentTests.cpp
    test/tcframe/deprecated/ioformat/LinesIOSegmentTests.cpp
    test/tcframe/deprecated/ioformat/ParsingExceptionTests.cpp
    test/tcframe/deprecated/ioformat/PrintingExceptionTests.cpp
    test/tcframe/deprecated/iovariable/HorizontalVectorTests.cpp
    test/tcframe/deprecated/iovariable/MatrixTests.cpp
    test/tcframe/deprecated/iovariable/ScalarTests.cpp
    test/tcframe/deprecated/iovariable/VerticalVectorTests.cpp
    test/tcframe/deprecated/os/ExecutionExceptionTests.cpp
    test/tcframe/deprecated/problem/BaseProblemTests.cpp
    test/tcframe/deprecated/problem/ConstraintsCollectorTests.cpp
    test/tcframe/deprecated/problem/ConstraintTests.cpp
    test/tcframe/deprecated/random/RandomTests.cpp
    test/tcframe/deprecated/runner/MacroTests.cpp
    test/tcframe/deprecated/submitter/SubmitterTests.cpp
)

add_executable(all_tests ${SOURCE_FILES})

target_link_libraries(all_tests
    gtest
    gmock
    ${CMAKE_THREAD_LIBS_INIT}
    gcov
)

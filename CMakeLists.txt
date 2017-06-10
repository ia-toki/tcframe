cmake_minimum_required(VERSION 2.6)
project(tcframe)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

find_package(Git REQUIRED)
find_package(Threads REQUIRED)

include(ExternalProject)

ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.0
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

set(INCLUDE
        include/tcframe/aggregator.hpp
        include/tcframe/aggregator/Aggregator.hpp
        include/tcframe/aggregator/AggregatorRegistry.hpp
        include/tcframe/aggregator/MinAggregator.hpp
        include/tcframe/aggregator/SumAggregator.hpp
        include/tcframe/evaluator.hpp
        include/tcframe/evaluator/BatchEvaluator.hpp
        include/tcframe/evaluator/EvaluationOptions.hpp
        include/tcframe/evaluator/EvaluationResult.hpp
        include/tcframe/evaluator/Evaluator.hpp
        include/tcframe/evaluator/EvaluatorConfig.hpp
        include/tcframe/evaluator/EvaluatorHelperRegistry.hpp
        include/tcframe/evaluator/EvaluatorRegistry.hpp
        include/tcframe/evaluator/InteractiveEvaluator.hpp
        include/tcframe/evaluator/GenerationResult.hpp
        include/tcframe/evaluator/communicator.hpp
        include/tcframe/evaluator/communicator/CommunicationResult.hpp
        include/tcframe/evaluator/communicator/Communicator.hpp
        include/tcframe/evaluator/scorer.hpp
        include/tcframe/evaluator/scorer/CustomScorer.hpp
        include/tcframe/evaluator/scorer/DiffScorer.hpp
        include/tcframe/evaluator/scorer/Scorer.hpp
        include/tcframe/evaluator/scorer/ScoringResult.hpp
        include/tcframe/generator.hpp
        include/tcframe/generator/DefaultGeneratorLogger.hpp
        include/tcframe/generator/GenerationException.hpp
        include/tcframe/generator/GenerationOptions.hpp
        include/tcframe/generator/Generator.hpp
        include/tcframe/generator/GeneratorLogger.hpp
        include/tcframe/generator/TestCaseGenerator.hpp
        include/tcframe/grader.hpp
        include/tcframe/grader/BriefGraderLogger.hpp
        include/tcframe/grader/DefaultGraderLogger.hpp
        include/tcframe/grader/Grader.hpp
        include/tcframe/grader/GraderLogger.hpp
        include/tcframe/grader/GraderLoggerFactory.hpp
        include/tcframe/grader/GradingOptions.hpp
        include/tcframe/grader/TestCaseGrader.hpp
        include/tcframe/io_manipulator.hpp
        include/tcframe/io_manipulator/GridIOSegmentManipulator.hpp
        include/tcframe/io_manipulator/IOManipulator.hpp
        include/tcframe/io_manipulator/LineIOSegmentManipulator.hpp
        include/tcframe/io_manipulator/LinesIOSegmentManipulator.hpp
        include/tcframe/io_manipulator/RawLineIOSegmentManipulator.hpp
        include/tcframe/io_manipulator/RawLinesIOSegmentManipulator.hpp
        include/tcframe/logger.hpp
        include/tcframe/logger/BaseLogger.hpp
        include/tcframe/logger/DefaultBaseLogger.hpp
        include/tcframe/logger/LoggerEngine.hpp
        include/tcframe/logger/SimpleLoggerEngine.hpp
        include/tcframe/os.hpp
        include/tcframe/os/ExecutionResult.hpp
        include/tcframe/os/ExecutionRequest.hpp
        include/tcframe/os/OperatingSystem.hpp
        include/tcframe/runner.hpp
        include/tcframe/runner/Args.hpp
        include/tcframe/runner/ArgsParser.hpp
        include/tcframe/runner/Runner.hpp
        include/tcframe/runner/RunnerLogger.hpp
        include/tcframe/runner/SlugParser.hpp
        include/tcframe/spec.hpp
        include/tcframe/spec/constraint.hpp
        include/tcframe/spec/constraint/Constraint.hpp
        include/tcframe/spec/constraint/ConstraintSuite.hpp
        include/tcframe/spec/constraint/Subtask.hpp
        include/tcframe/spec/core.hpp
        include/tcframe/spec/core/BaseTestSpec.hpp
        include/tcframe/spec/core/BaseProblemSpec.hpp
        include/tcframe/spec/core/GradingConfig.hpp
        include/tcframe/spec/core/Magic.hpp
        include/tcframe/spec/core/MultipleTestCasesConfig.hpp
        include/tcframe/spec/core/SeedSetter.hpp
        include/tcframe/spec/core/Spec.hpp
        include/tcframe/spec/core/StyleConfig.hpp
        include/tcframe/spec/io.hpp
        include/tcframe/spec/io/GridIOSegment.hpp
        include/tcframe/spec/io/IOFormat.hpp
        include/tcframe/spec/io/IOSegment.hpp
        include/tcframe/spec/io/LineIOSegment.hpp
        include/tcframe/spec/io/LinesIOSegment.hpp
        include/tcframe/spec/io/RawLineIOSegment.hpp
        include/tcframe/spec/io/RawLinesIOSegment.hpp
        include/tcframe/spec/random.hpp
        include/tcframe/spec/random/Random.hpp
        include/tcframe/spec/testcase.hpp
        include/tcframe/spec/testcase/OfficialTestCaseData.hpp
        include/tcframe/spec/testcase/SampleTestCaseData.hpp
        include/tcframe/spec/testcase/TestCase.hpp
        include/tcframe/spec/testcase/TestCaseData.hpp
        include/tcframe/spec/testcase/TestGroup.hpp
        include/tcframe/spec/testcase/TestSuite.hpp
        include/tcframe/spec/variable.hpp
        include/tcframe/spec/variable/Scalar.hpp
        include/tcframe/spec/variable/Matrix.hpp
        include/tcframe/spec/variable/TokenFormatter.hpp
        include/tcframe/spec/variable/Variable.hpp
        include/tcframe/spec/variable/Vector.hpp
        include/tcframe/spec/variable/WhitespaceManipulator.hpp
        include/tcframe/util.hpp
        include/tcframe/util/NotImplementedException.hpp
        include/tcframe/util/StringUtils.hpp
        include/tcframe/util/optional.hpp
        include/tcframe/verdict.hpp
        include/tcframe/verdict/Verdict.hpp
        include/tcframe/verdict/VerdictCreator.hpp
        include/tcframe/verdict/VerdictStatus.hpp
        include/tcframe/verifier.hpp
        include/tcframe/verifier/ConstraintsVerificationResult.hpp
        include/tcframe/verifier/MultipleTestCasesConstraintsVerificationResult.hpp
        include/tcframe/verifier/Verifier.hpp
)

set(TEST_UNIT
        test/unit/tcframe/helper.hpp
        test/unit/tcframe/mock.hpp
        test/unit/tcframe/aggregator/MinAggregatorTests.cpp
        test/unit/tcframe/aggregator/MockAggregator.hpp
        test/unit/tcframe/aggregator/MockAggregatorRegistry.hpp
        test/unit/tcframe/aggregator/SumAggregatorTests.cpp
        test/unit/tcframe/evaluator/BatchEvaluatorTests.cpp
        test/unit/tcframe/evaluator/InteractiveEvaluatorTests.cpp
        test/unit/tcframe/evaluator/MockEvaluator.hpp
        test/unit/tcframe/evaluator/MockEvaluatorRegistry.hpp
        test/unit/tcframe/evaluator/communicator/CommunicatorTests.cpp
        test/unit/tcframe/evaluator/communicator/MockCommunicator.hpp
        test/unit/tcframe/evaluator/scorer/CustomScorerTests.cpp
        test/unit/tcframe/evaluator/scorer/MockScorer.hpp
        test/unit/tcframe/generator/DefaultGeneratorLoggerTests.cpp
        test/unit/tcframe/generator/GeneratorTests.cpp
        test/unit/tcframe/generator/MockGenerator.hpp
        test/unit/tcframe/generator/MockGeneratorLogger.hpp
        test/unit/tcframe/generator/MockTestCaseGenerator.hpp
        test/unit/tcframe/generator/TestCaseGeneratorTests.cpp
        test/unit/tcframe/grader/BriefGraderLoggerTests.cpp
        test/unit/tcframe/grader/DefaultGraderLoggerTests.cpp
        test/unit/tcframe/grader/GraderTests.cpp
        test/unit/tcframe/grader/MockGrader.hpp
        test/unit/tcframe/grader/MockGraderLogger.hpp
        test/unit/tcframe/grader/MockGraderLoggerFactory.hpp
        test/unit/tcframe/grader/MockTestCaseGrader.hpp
        test/unit/tcframe/grader/TestCaseGraderTests.cpp
        test/unit/tcframe/io_manipulator/GridIOSegmentManipulatorTests.cpp
        test/unit/tcframe/io_manipulator/IOManipulatorTests.cpp
        test/unit/tcframe/io_manipulator/LineIOSegmentManipulatorTests.cpp
        test/unit/tcframe/io_manipulator/LinesIOSegmentManipulatorTests.cpp
        test/unit/tcframe/io_manipulator/MockIOManipulator.hpp
        test/unit/tcframe/io_manipulator/RawLineIOSegmentManipulatorTests.cpp
        test/unit/tcframe/io_manipulator/RawLinesIOSegmentManipulatorTests.cpp
        test/unit/tcframe/logger/DefaultBaseLogggerTests.cpp
        test/unit/tcframe/logger/MockLoggerEngine.hpp
        test/unit/tcframe/logger/SimpleLoggerEngineTests.cpp
        test/unit/tcframe/os/MockOperatingSystem.hpp
        test/unit/tcframe/runner/ArgsParserTests.cpp
        test/unit/tcframe/runner/BatchRunnerTests.cpp
        test/unit/tcframe/runner/BaseRunnerTests.hpp
        test/unit/tcframe/runner/InteractiveRunnerTests.cpp
        test/unit/tcframe/runner/MockRunnerLogger.hpp
        test/unit/tcframe/runner/RunnerTests.cpp
        test/unit/tcframe/runner/RunnerLoggerTests.cpp
        test/unit/tcframe/runner/SlugParserTests.cpp
        test/unit/tcframe/spec/constraint/ConstraintSuiteTests.cpp
        test/unit/tcframe/spec/constraint/ConstraintSuiteBuilderTests.cpp
        test/unit/tcframe/spec/core/BaseTestSpecTests.cpp
        test/unit/tcframe/spec/core/BaseProblemSpecTests.cpp
        test/unit/tcframe/spec/core/MagicTests.cpp
        test/unit/tcframe/spec/core/MockSeedSetter.hpp
        test/unit/tcframe/spec/io/GridIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/IOFormatBuilderTests.cpp
        test/unit/tcframe/spec/io/LineIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/LinesIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/RawLineIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/RawLinesIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/random/RandomTests.cpp
        test/unit/tcframe/spec/testcase/TestCaseTests.cpp
        test/unit/tcframe/spec/testcase/TestGroupTests.cpp
        test/unit/tcframe/spec/testcase/TestSuiteBuilderTests.cpp
        test/unit/tcframe/spec/variable/MatrixTests.cpp
        test/unit/tcframe/spec/variable/ScalarTests.cpp
        test/unit/tcframe/spec/variable/TokenFormatterTests.cpp
        test/unit/tcframe/spec/variable/VariableTests.cpp
        test/unit/tcframe/spec/variable/VectorTests.cpp
        test/unit/tcframe/spec/variable/WhitespaceManipulatorTests.cpp
        test/unit/tcframe/util/OptionalTests.cpp
        test/unit/tcframe/util/StringUtilsTests.cpp
        test/unit/tcframe/util/TestUtils.hpp
        test/unit/tcframe/verdict/MockVerdictCreator.hpp
        test/unit/tcframe/verdict/VerdictCreatorTests.cpp
        test/unit/tcframe/verdict/VerdictTests.cpp
        test/unit/tcframe/verifier/MockVerifier.hpp
        test/unit/tcframe/verifier/VerifierTests.cpp
)

add_executable(test_unit ${INCLUDE} ${TEST_UNIT})

target_link_libraries(test_unit
        gtest
        gmock
        ${CMAKE_THREAD_LIBS_INIT}
        gcov
)

set(TEST_INTEGRATION
        test/integration/tcframe/evaluator/communicator/CommunicatorIntegrationTests.cpp
        test/integration/tcframe/evaluator/scorer/CustomScorerIntegrationTests.cpp
        test/integration/tcframe/evaluator/scorer/DiffScorerIntegrationTests.cpp
        test/integration/tcframe/os/OperatingSystemIntegrationTests.cpp
)

add_executable(test_integration ${INCLUDE} ${TEST_INTEGRATION})

target_link_libraries(test_integration
        gtest
        gmock
        ${CMAKE_THREAD_LIBS_INIT}
        gcov
)

add_custom_command(
        TARGET test_integration
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/integration/resources $<TARGET_FILE_DIR:test_integration>/test-integration
)

set(TEST_ETE
        test/ete/tcframe/BaseEteTests.cpp
        test/ete/tcframe/GenerationEteTests.cpp
        test/ete/tcframe/GradingEteTests.cpp
)

add_executable(test_ete ${TEST_ETE})

target_link_libraries(test_ete
        gtest
        gmock
        ${CMAKE_THREAD_LIBS_INIT}
        gcov
)

add_custom_command(
        TARGET test_ete
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include $<TARGET_FILE_DIR:test_ete>/tcframe/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src $<TARGET_FILE_DIR:test_ete>/tcframe/src
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:test_ete>/tcframe/scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/ete/resources $<TARGET_FILE_DIR:test_ete>/test-ete
)

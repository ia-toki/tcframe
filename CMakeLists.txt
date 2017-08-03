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
        include/tcframe/runner/aggregator.hpp
        include/tcframe/runner/aggregator/Aggregator.hpp
        include/tcframe/runner/aggregator/AggregatorRegistry.hpp
        include/tcframe/runner/aggregator/MinAggregator.hpp
        include/tcframe/runner/aggregator/SumAggregator.hpp
        include/tcframe/runner/core.hpp
        include/tcframe/runner/core/Args.hpp
        include/tcframe/runner/core/ArgsParser.hpp
        include/tcframe/runner/core/Runner.hpp
        include/tcframe/runner/core/SlugParser.hpp
        include/tcframe/runner/evaluator.hpp
        include/tcframe/runner/evaluator/BatchEvaluator.hpp
        include/tcframe/runner/evaluator/EvaluationOptions.hpp
        include/tcframe/runner/evaluator/EvaluationResult.hpp
        include/tcframe/runner/evaluator/Evaluator.hpp
        include/tcframe/runner/evaluator/EvaluatorConfig.hpp
        include/tcframe/runner/evaluator/EvaluatorHelperRegistry.hpp
        include/tcframe/runner/evaluator/EvaluatorRegistry.hpp
        include/tcframe/runner/evaluator/InteractiveEvaluator.hpp
        include/tcframe/runner/evaluator/GenerationResult.hpp
        include/tcframe/runner/evaluator/communicator.hpp
        include/tcframe/runner/evaluator/communicator/CommunicationResult.hpp
        include/tcframe/runner/evaluator/communicator/Communicator.hpp
        include/tcframe/runner/evaluator/scorer.hpp
        include/tcframe/runner/evaluator/scorer/CustomScorer.hpp
        include/tcframe/runner/evaluator/scorer/DiffScorer.hpp
        include/tcframe/runner/evaluator/scorer/Scorer.hpp
        include/tcframe/runner/evaluator/scorer/ScoringResult.hpp
        include/tcframe/runner/grader.hpp
        include/tcframe/runner/grader/BriefGraderLogger.hpp
        include/tcframe/runner/grader/DefaultGraderLogger.hpp
        include/tcframe/runner/grader/Grader.hpp
        include/tcframe/runner/grader/GraderLogger.hpp
        include/tcframe/runner/grader/GraderLoggerFactory.hpp
        include/tcframe/runner/grader/GradingOptions.hpp
        include/tcframe/runner/grader/TestCaseGrader.hpp
        include/tcframe/runner/generator.hpp
        include/tcframe/runner/generator/DefaultGeneratorLogger.hpp
        include/tcframe/runner/generator/GenerationException.hpp
        include/tcframe/runner/generator/GenerationOptions.hpp
        include/tcframe/runner/generator/Generator.hpp
        include/tcframe/runner/generator/GeneratorLogger.hpp
        include/tcframe/runner/generator/TestCaseGenerator.hpp
        include/tcframe/runner/logger.hpp
        include/tcframe/runner/logger/RunnerLogger.hpp
        include/tcframe/runner/verdict.hpp
        include/tcframe/runner/verdict/Verdict.hpp
        include/tcframe/runner/verdict/VerdictCreator.hpp
        include/tcframe/runner/verdict/VerdictStatus.hpp
        include/tcframe/spec.hpp
        include/tcframe/spec/config.hpp
        include/tcframe/spec/config/GradingConfig.hpp
        include/tcframe/spec/config/MultipleTestCasesConfig.hpp
        include/tcframe/spec/config/StyleConfig.hpp
        include/tcframe/spec/constraint.hpp
        include/tcframe/spec/constraint/Constraint.hpp
        include/tcframe/spec/constraint/ConstraintSuite.hpp
        include/tcframe/spec/constraint/Subtask.hpp
        include/tcframe/spec/core.hpp
        include/tcframe/spec/core/BaseTestSpec.hpp
        include/tcframe/spec/core/BaseProblemSpec.hpp
        include/tcframe/spec/core/Magic.hpp
        include/tcframe/spec/core/SeedSetter.hpp
        include/tcframe/spec/core/Spec.hpp
        include/tcframe/spec/io.hpp
        include/tcframe/spec/io/GridIOSegment.hpp
        include/tcframe/spec/io/GridIOSegmentManipulator.hpp
        include/tcframe/spec/io/IOFormat.hpp
        include/tcframe/spec/io/IOManipulator.hpp
        include/tcframe/spec/io/IOSegment.hpp
        include/tcframe/spec/io/LineIOSegment.hpp
        include/tcframe/spec/io/LineIOSegmentManipulator.hpp
        include/tcframe/spec/io/LinesIOSegment.hpp
        include/tcframe/spec/io/LinesIOSegmentManipulator.hpp
        include/tcframe/spec/io/RawLineIOSegment.hpp
        include/tcframe/spec/io/RawLineIOSegmentManipulator.hpp
        include/tcframe/spec/io/RawLinesIOSegment.hpp
        include/tcframe/spec/io/RawLinesIOSegmentManipulator.hpp
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
        include/tcframe/spec/verifier.hpp
        include/tcframe/spec/verifier/ConstraintsVerificationResult.hpp
        include/tcframe/spec/verifier/MultipleTestCasesConstraintsVerificationResult.hpp
        include/tcframe/spec/verifier/Verifier.hpp
        include/tcframe/util.hpp
        include/tcframe/util/NotImplementedException.hpp
        include/tcframe/util/StringUtils.hpp
        include/tcframe/util/optional.hpp
)

set(TEST_UNIT
        test/unit/tcframe/helper.hpp
        test/unit/tcframe/mock.hpp
        test/unit/tcframe/logger/DefaultBaseLogggerTests.cpp
        test/unit/tcframe/logger/MockLoggerEngine.hpp
        test/unit/tcframe/logger/SimpleLoggerEngineTests.cpp
        test/unit/tcframe/os/MockOperatingSystem.hpp
        test/unit/tcframe/runner/aggregator/MinAggregatorTests.cpp
        test/unit/tcframe/runner/aggregator/MockAggregator.hpp
        test/unit/tcframe/runner/aggregator/MockAggregatorRegistry.hpp
        test/unit/tcframe/runner/aggregator/SumAggregatorTests.cpp
        test/unit/tcframe/runner/core/ArgsParserTests.cpp
        test/unit/tcframe/runner/core/BatchRunnerTests.cpp
        test/unit/tcframe/runner/core/BaseRunnerTests.hpp
        test/unit/tcframe/runner/core/InteractiveRunnerTests.cpp
        test/unit/tcframe/runner/core/RunnerTests.cpp
        test/unit/tcframe/runner/core/SlugParserTests.cpp
        test/unit/tcframe/runner/evaluator/BatchEvaluatorTests.cpp
        test/unit/tcframe/runner/evaluator/InteractiveEvaluatorTests.cpp
        test/unit/tcframe/runner/evaluator/MockEvaluator.hpp
        test/unit/tcframe/runner/evaluator/MockEvaluatorRegistry.hpp
        test/unit/tcframe/runner/evaluator/communicator/CommunicatorTests.cpp
        test/unit/tcframe/runner/evaluator/communicator/MockCommunicator.hpp
        test/unit/tcframe/runner/evaluator/scorer/CustomScorerTests.cpp
        test/unit/tcframe/runner/evaluator/scorer/MockScorer.hpp
        test/unit/tcframe/runner/generator/DefaultGeneratorLoggerTests.cpp
        test/unit/tcframe/runner/generator/GeneratorTests.cpp
        test/unit/tcframe/runner/generator/MockGenerator.hpp
        test/unit/tcframe/runner/generator/MockGeneratorLogger.hpp
        test/unit/tcframe/runner/generator/MockTestCaseGenerator.hpp
        test/unit/tcframe/runner/generator/TestCaseGeneratorTests.cpp
        test/unit/tcframe/runner/grader/BriefGraderLoggerTests.cpp
        test/unit/tcframe/runner/grader/DefaultGraderLoggerTests.cpp
        test/unit/tcframe/runner/grader/GraderTests.cpp
        test/unit/tcframe/runner/grader/MockGrader.hpp
        test/unit/tcframe/runner/grader/MockGraderLogger.hpp
        test/unit/tcframe/runner/grader/MockGraderLoggerFactory.hpp
        test/unit/tcframe/runner/grader/MockTestCaseGrader.hpp
        test/unit/tcframe/runner/grader/TestCaseGraderTests.cpp
        test/unit/tcframe/runner/logger/MockRunnerLogger.hpp
        test/unit/tcframe/runner/logger/RunnerLoggerTests.cpp
        test/unit/tcframe/runner/verdict/MockVerdictCreator.hpp
        test/unit/tcframe/runner/verdict/VerdictCreatorTests.cpp
        test/unit/tcframe/runner/verdict/VerdictTests.cpp
        test/unit/tcframe/spec/constraint/ConstraintSuiteTests.cpp
        test/unit/tcframe/spec/constraint/ConstraintSuiteBuilderTests.cpp
        test/unit/tcframe/spec/core/BaseTestSpecTests.cpp
        test/unit/tcframe/spec/core/BaseProblemSpecTests.cpp
        test/unit/tcframe/spec/core/MagicTests.cpp
        test/unit/tcframe/spec/core/MockSeedSetter.hpp
        test/unit/tcframe/spec/io/GridIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/GridIOSegmentManipulatorTests.cpp
        test/unit/tcframe/spec/io/IOFormatBuilderTests.cpp
        test/unit/tcframe/spec/io/IOManipulatorTests.cpp
        test/unit/tcframe/spec/io/LineIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/LineIOSegmentManipulatorTests.cpp
        test/unit/tcframe/spec/io/LinesIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/LinesIOSegmentManipulatorTests.cpp
        test/unit/tcframe/spec/io/MockIOManipulator.hpp
        test/unit/tcframe/spec/io/RawLineIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/RawLineIOSegmentManipulatorTests.cpp
        test/unit/tcframe/spec/io/RawLinesIOSegmentBuilderTests.cpp
        test/unit/tcframe/spec/io/RawLinesIOSegmentManipulatorTests.cpp
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
        test/unit/tcframe/spec/verifier/MockVerifier.hpp
        test/unit/tcframe/spec/verifier/VerifierTests.cpp
        test/unit/tcframe/util/OptionalTests.cpp
        test/unit/tcframe/util/StringUtilsTests.cpp
        test/unit/tcframe/util/TestUtils.hpp
)

add_executable(test_unit ${INCLUDE} ${TEST_UNIT})

target_link_libraries(test_unit
        gtest
        gmock
        ${CMAKE_THREAD_LIBS_INIT}
        gcov
)

set(TEST_INTEGRATION
        test/integration/tcframe/runner/evaluator/communicator/CommunicatorIntegrationTests.cpp
        test/integration/tcframe/runner/evaluator/scorer/CustomScorerIntegrationTests.cpp
        test/integration/tcframe/runner/evaluator/scorer/DiffScorerIntegrationTests.cpp
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

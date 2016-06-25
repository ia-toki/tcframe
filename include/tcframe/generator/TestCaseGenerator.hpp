#pragma once

#include <functional>
#include <set>
#include <string>

#include "GeneratorLogger.hpp"
#include "TestCase.hpp"
#include "TestCaseGenerationResult.hpp"
#include "tcframe/core.hpp"
#include "tcframe/failure.hpp"
#include "tcframe/io.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"
#include "tcframe/verifier.hpp"

using std::function;
using std::set;
using std::string;

namespace tcframe {

class TestCaseGenerator {
private:
    ConstraintSuiteVerifier* verifier_;
    IOManipulator* ioManipulator_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            ConstraintSuiteVerifier* verifier,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : verifier_(verifier)
            , ioManipulator_(ioManipulator)
            , os_(os)
            , logger_(logger)
    {}

    virtual TestCaseGenerationResult generate(const TestCase& testCase, const TestConfig& testConfig) {
        logger_->logTestCaseIntroduction(testCase.name());
        TestCaseGenerationResult result = doGenerate(testCase, testConfig);
        logger_->logTestCaseResult(testCase.description(), result);
        return result;
    }

private:
    TestCaseGenerationResult doGenerate(const TestCase& testCase, const TestConfig& config) {
        string inputFilename = config.testCasesDir() + "/" + testCase.name() + ".in";
        string outputFilename = config.testCasesDir() + "/" + testCase.name() + ".out";

        try {
            apply(testCase.applier());
            verify(testCase.subtaskIds());
            generateInput(inputFilename);
            generateOutput(inputFilename, outputFilename, config.solutionCommand());
        } catch (ComplexFailureException& e) {
            return TestCaseGenerationResult::failedResult(e.failure());
        } catch (runtime_error& e) {
            return TestCaseGenerationResult::failedResult(new SimpleFailure(e.what()));
        }

        return TestCaseGenerationResult::successfulResult();
    }

    void apply(const function<void()>& applier) {
        applier();
    }

    void verify(const set<int>& subtaskIds) {
        VerificationResult result = verifier_->verify(subtaskIds);
        if (!result.isValid()) {
            throw ComplexFailureException(new VerificationFailure(result));
        }
    }

    void generateInput(const string& inputFilename) {
        ostream* testCaseInput = os_->openForWriting(inputFilename);
        ioManipulator_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateOutput(const string& inputFilename, const string& outputFilename, const string& solutionCommand) {
        ExecutionResult result = os_->execute(solutionCommand, inputFilename, outputFilename, "_error.out");
        ioManipulator_->parseOutput(result.outputStream());
    }
};

}

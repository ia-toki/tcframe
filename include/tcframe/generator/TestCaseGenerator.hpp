#pragma once

#include <iostream>
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

using std::endl;
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

    virtual TestCaseGenerationResult generate(const TestCase& testCase, const CoreConfig& coreConfig) {
        logger_->logTestCaseIntroduction(testCase.id());
        TestCaseGenerationResult result = doGenerate(testCase, coreConfig);
        logger_->logTestCaseResult(testCase.description(), result);
        return result;
    }

private:
    TestCaseGenerationResult doGenerate(const TestCase& testCase, const CoreConfig& coreConfig) {
        string inputFilename = coreConfig.testConfig().testCasesDir() + "/" + testCase.id() + ".in";
        string outputFilename = coreConfig.testConfig().testCasesDir() + "/" + testCase.id() + ".out";

        try {
            apply(testCase.applier());
            verify(testCase.subtaskIds());
            generateInput(inputFilename, coreConfig.problemConfig());
            generateOutput(inputFilename, outputFilename, coreConfig.testConfig().solutionCommand());
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
        ConstraintsVerificationResult result = verifier_->verifyConstraints(subtaskIds);
        if (!result.isValid()) {
            throw ComplexFailureException(new ConstraintsVerificationFailure(result));
        }
    }

    void generateInput(const string& inputFilename, const ProblemConfig& problemConfig) {
        ostream* testCaseInput = os_->openForWriting(inputFilename);
        if (problemConfig.multipleTestCasesCount() != nullptr) {
            *testCaseInput << "1" << endl;
        }
        ioManipulator_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateOutput(const string& inputFilename, const string& outputFilename, const string& solutionCommand) {
        ExecutionResult result = os_->execute(solutionCommand, inputFilename, outputFilename, "_error.out");
        ioManipulator_->parseOutput(result.outputStream());
    }
};

}

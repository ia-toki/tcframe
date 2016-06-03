#pragma once

#include <functional>
#include <set>
#include <string>

#include "TestCaseData.hpp"
#include "TestCaseGenerationException.hpp"
#include "TestCaseGenerationResult.hpp"
#include "VerificationFailure.hpp"
#include "tcframe/core.hpp"
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

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            ConstraintSuiteVerifier* verifier,
            IOManipulator* ioManipulator,
            OperatingSystem* os)
            : verifier_(verifier)
            , ioManipulator_(ioManipulator)
            ,  os_(os)
    {}

    virtual TestCaseGenerationResult generate(
            const TestCaseData& data,
            const function<void()>& closure,
            const GeneratorConfig& config) {

        return doGenerate(data, closure, config);
    }

private:
    TestCaseGenerationResult doGenerate(
            const TestCaseData& data,
            const function<void()>& applier,
            const GeneratorConfig& config) {

        string inputFilename = config.testCasesDir() + "/" + data.name() + ".in";
        string outputFilename = config.testCasesDir() + "/" + data.name() + ".out";

        try {
            apply(applier);
            verify(data.constraintGroupIds());
            generateInput(inputFilename);
            generateOutput(inputFilename, outputFilename, config.solutionCommand());
        } catch (TestCaseGenerationException& e) {
            return TestCaseGenerationResult::failedResult(e.failure());
        } catch (...) {

        }

        return TestCaseGenerationResult::successfulResult();
    }

    void apply(const function<void()>& applier) {
        applier();
    }

    void verify(const set<int>& constraintGroupIds) {
        VerificationResult result = verifier_->verify(constraintGroupIds);
        if (!result.isValid()) {
            throw TestCaseGenerationException(new VerificationFailure(result));
        }
    }

    void generateInput(const string& inputFilename) {
        ostream* testCaseInput = os_->openForWriting(inputFilename);
        ioManipulator_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateOutput(const string& inputFilename, const string& outputFilename, const string& solutionCommand) {
        os_->execute(solutionCommand, inputFilename, outputFilename, "_error.out");
    }
};

}

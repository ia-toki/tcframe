#pragma once

#include <functional>
#include <set>
#include <string>

#include "TestCaseData.hpp"
#include "TestCaseGenerationException.hpp"
#include "TestCaseGenerationResult.hpp"
#include "VerificationFailure.hpp"
#include "tcframe/config.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"
#include "tcframe/variable.hpp"
#include "tcframe/verification.hpp"

using std::function;
using std::set;
using std::string;

namespace tcframe {

class TestCaseGenerator {
private:
    ConstraintSuiteVerifier* constraintSuiteVerifier_;
    IOVariablesPrinter* ioVariablePrinter_;
    OperatingSystem* os_;

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            ConstraintSuiteVerifier* constraintSuiteVerifier,
            IOVariablesPrinter* ioVariablePrinter,
            OperatingSystem* os)
            : constraintSuiteVerifier_(constraintSuiteVerifier)
            , ioVariablePrinter_(ioVariablePrinter)
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
        ConstraintSuiteVerificationResult result = constraintSuiteVerifier_->verify(constraintGroupIds);
        if (!result.isValid()) {
            throw TestCaseGenerationException(new VerificationFailure(result));
        }
    }

    void generateInput(const string& inputFilename) {
        ostream* testCaseInput = os_->openForWriting(inputFilename);
        ioVariablePrinter_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateOutput(const string& inputFilename, const string& outputFilename, const string& solutionCommand) {
        os_->execute(solutionCommand, inputFilename, outputFilename, "_error.out");
    }
};

}

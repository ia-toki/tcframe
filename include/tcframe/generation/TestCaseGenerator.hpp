#pragma once

#include <functional>
#include <set>
#include <string>

#include "tcframe/config/GeneratorConfig.hpp"
#include "tcframe/generation/TestCaseData.hpp"
#include "tcframe/generation/TestCaseGenerationException.hpp"
#include "tcframe/generation/TestCaseGenerationResult.hpp"
#include "tcframe/generation/VerificationFailure.hpp"
#include "tcframe/os/OperatingSystem.hpp"
#include "tcframe/testcase/TestCase.hpp"
#include "tcframe/variable/IOVariablePrinter.hpp"
#include "tcframe/verification/ConstraintSuiteVerifier.hpp"

using std::function;
using std::set;
using std::string;

namespace tcframe { namespace experimental {

class TestCaseGenerator {
private:
    ConstraintSuiteVerifier* constraintSuiteVerifier_;
    IOVariablePrinter* ioVariablePrinter_;
    OperatingSystem* os_;

public:
    TestCaseGenerator(
            ConstraintSuiteVerifier* constraintSuiteVerifier,
            IOVariablePrinter* ioVariablePrinter,
            OperatingSystem* os)
            : constraintSuiteVerifier_(constraintSuiteVerifier)
            , ioVariablePrinter_(ioVariablePrinter)
            ,  os_(os)
    {}

    TestCaseGenerationResult generate(
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

}}

#pragma once

#include <iostream>
#include <functional>
#include <set>
#include <string>

#include "GenerationException.hpp"
#include "GeneratorConfig.hpp"
#include "GeneratorLogger.hpp"
#include "tcframe/io_manipulator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/testcase.hpp"
#include "tcframe/verifier.hpp"

using std::endl;
using std::function;
using std::set;
using std::string;

namespace tcframe {

class TestCaseGenerator {
private:
    Verifier* verifier_;
    IOManipulator* ioManipulator_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            Verifier* verifier,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : verifier_(verifier)
            , ioManipulator_(ioManipulator)
            , os_(os)
            , logger_(logger) {}

    virtual bool generate(const TestCase& testCase, const GeneratorConfig& config) {
        logger_->logTestCaseIntroduction(testCase.id());

        string inputFilename = config.testCasesDir() + "/" + testCase.id() + ".in";
        string outputFilename = config.testCasesDir() + "/" + testCase.id() + ".out";

        try {
            apply(testCase.applier());
            verify(testCase.subtaskIds());
            generateInput(inputFilename, config);
            generateOutput(inputFilename, outputFilename, config.solutionCommand());
        } catch (GenerationException& e) {
            logger_->logTestCaseFailedResult(testCase.description());
            e.callback()();
            return false;
        } catch (runtime_error& e) {
            logger_->logTestCaseFailedResult(testCase.description());
            logger_->logSimpleFailure(e.what());
            return false;
        }

        logger_->logTestCaseSuccessfulResult();
        return true;
    }

private:
    void apply(const function<void()>& applier) {
        applier();
    }

    void verify(const set<int>& subtaskIds) {
        ConstraintsVerificationResult result = verifier_->verifyConstraints(subtaskIds);
        if (!result.isValid()) {
            throw GenerationException([=] {logger_->logConstraintsVerificationFailure(result);});
        }
    }

    void generateInput(const string& inputFilename, const GeneratorConfig& config) {
        ostream* testCaseInput = os_->openForWriting(inputFilename);
        if (config.multipleTestCasesCount() != nullptr) {
            *testCaseInput << "1" << endl;
        }
        ioManipulator_->printInput(testCaseInput);
        os_->closeOpenedWritingStream(testCaseInput);
    }

    void generateOutput(const string& inputFilename, const string& outputFilename, const string& solutionCommand) {
        ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                .setCommand(solutionCommand)
                .setInputFilename(inputFilename)
                .setOutputFilename(outputFilename)
                .setErrorFilename("_error.out")
                .build());
        if (!result.info().isSuccessful()) {
            throw GenerationException([=] {logger_->logSolutionExecutionFailure(result);});
        }
        ioManipulator_->parseOutput(result.outputStream());
    }
};

}

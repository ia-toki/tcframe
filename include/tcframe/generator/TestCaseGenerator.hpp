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
            applyInput(testCase);
            verifyInput(testCase);
            generateInput(testCase, inputFilename, config);
            generateAndApplyOutput(testCase, inputFilename, outputFilename, config);
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
    void applyInput(const TestCase& testCase) {
        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            SampleTestCaseData* data = (SampleTestCaseData*) testCase.data();
            istringstream input(data->input());
            ioManipulator_->parseInput(&input);
        } else {
            OfficialTestCaseData* data = (OfficialTestCaseData*) testCase.data();
            data->closure()();
        }
    }

    void verifyInput(const TestCase& testCase) {
        ConstraintsVerificationResult result = verifier_->verifyConstraints(testCase.subtaskIds());
        if (!result.isValid()) {
            throw GenerationException([=] {logger_->logConstraintsVerificationFailure(result);});
        }
    }

    void generateInput(const TestCase& testCase, const string& inputFilename, const GeneratorConfig& config) {
        ostream* input = os_->openForWriting(inputFilename);
        if (config.multipleTestCasesCount() != nullptr) {
            *input << "1" << endl;
        }

        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            SampleTestCaseData* data = (SampleTestCaseData*) testCase.data();
            *input << data->input();
        } else {
            ioManipulator_->printInput(input);
        }
        os_->closeOpenedWritingStream(input);
    }

    void generateAndApplyOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GeneratorConfig& config) {

        istream* output;

        optional<string> outputString;
        if (testCase.data()->type() == TestCaseDataType::SAMPLE ) {
            outputString = ((SampleTestCaseData*) testCase.data())->output();
        }

        if (outputString) {
            ostream* _output = os_->openForWriting(outputFilename);
            *_output << outputString.value();
            os_->closeOpenedWritingStream(_output);

            output = new istringstream(outputString.value());
        } else {
            ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                    .setCommand(config.solutionCommand())
                    .setInputFilename(inputFilename)
                    .setOutputFilename(outputFilename)
                    .setErrorFilename("_error.out")
                    .build());
            if (!result.info().isSuccessful()) {
                throw GenerationException([=] { logger_->logSolutionExecutionFailure(result); });
            }
            output = result.outputStream();
        }

        ioManipulator_->parseOutput(output);
    }
};

}

#pragma once

#include <iostream>
#include <functional>
#include <set>
#include <string>
#include <type_traits>

#include "GenerationException.hpp"
#include "GeneratorConfig.hpp"
#include "GeneratorLogger.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/io_manipulator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/verifier.hpp"

using std::char_traits;
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
    Evaluator* evaluator_;
    GeneratorLogger* logger_;

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            Verifier* verifier,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            Evaluator* evaluator,
            GeneratorLogger* logger)
            : verifier_(verifier)
            , ioManipulator_(ioManipulator)
            , os_(os)
            , evaluator_(evaluator)
            , logger_(logger) {}

    virtual bool generate(const TestCase& testCase, const GeneratorConfig& config) {
        logger_->logTestCaseIntroduction(testCase.id());

        string inputFilename = config.outputDir() + "/" + testCase.id() + ".in";
        string outputFilename = config.outputDir() + "/" + testCase.id() + ".out";

        try {
            applyInput(testCase);
            verifyInput(testCase);
            generateInput(testCase, inputFilename, config);
            evaluateAndApplyOutput(testCase, inputFilename, outputFilename, config);
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
        modifyInputForMultipleTestCases(input, config);

        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            SampleTestCaseData* data = (SampleTestCaseData*) testCase.data();
            *input << data->input();
        } else {
            ioManipulator_->printInput(input);
        }
        os_->closeOpenedWritingStream(input);
    }

    void evaluateAndApplyOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GeneratorConfig& config) {

        istream* output;

        optional<string> maybeOutputString;
        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            maybeOutputString = ((SampleTestCaseData*) testCase.data())->output();
        }

        if (maybeOutputString) {
            string outputString = maybeOutputString.value();
            output = new istringstream(outputString);
            modifySampleOutputStringForMultipleTestCases(outputString, config);

            ostream* outputFile = os_->openForWriting(outputFilename);
            *outputFile << outputString;
            os_->closeOpenedWritingStream(outputFile);
        } else {
            EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
                    .setSolutionCommand(config.solutionCommand())
                    .build();

            EvaluationResult evaluationResult = evaluator_->evaluate(inputFilename, outputFilename, evaluatorConfig);
            ExecutionResult executionResult = evaluationResult.executionResult();

            if (!executionResult.info().isSuccessful()) {
                throw GenerationException([=] { logger_->logSolutionExecutionFailure(executionResult); });
            }
            output = executionResult.outputStream();

            modifyOutputForMultipleTestCases(output, config);
        }

        ioManipulator_->parseOutput(output);
    }

    void modifyInputForMultipleTestCases(ostream* input, const GeneratorConfig& config) {
        if (config.multipleTestCasesCounter() != nullptr) {
            int testCaseNo = 1;
            *input << testCaseNo << endl;
        }
    }

    void modifySampleOutputStringForMultipleTestCases(string& outputString, const GeneratorConfig& config) {
        if (config.multipleTestCasesCounter() != nullptr && config.multipleTestCasesFirstOutputPrefix()) {
            outputString = config.multipleTestCasesFirstOutputPrefix().value() + outputString;
        }
    }

    void modifyOutputForMultipleTestCases(istream* output, const GeneratorConfig& config) {
        if (config.multipleTestCasesCounter() != nullptr && config.multipleTestCasesFirstOutputPrefix()) {
            string prefix = config.multipleTestCasesOutputPrefix().value();
            string firstPrefix = config.multipleTestCasesFirstOutputPrefix().value();
            for (char p : firstPrefix) {
                int c = output->peek();
                if (c == char_traits<char>::eof() || (char) c != p) {
                    throw runtime_error("Output must start with \"" + prefix + "\"");
                }
                output->get();
            }
        }
    }
};

}

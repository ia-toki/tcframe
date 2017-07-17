#pragma once

#include <iostream>
#include <functional>
#include <set>
#include <string>
#include <type_traits>

#include "GenerationException.hpp"
#include "GenerationOptions.hpp"
#include "GeneratorLogger.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/io_manipulator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/verdict.hpp"

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

    virtual bool generate(const TestCase& testCase, const GenerationOptions& options) {
        logger_->logTestCaseIntroduction(testCase.name());

        string inputFilename = options.outputDir() + "/" + testCase.name() + ".in";
        string outputFilename = options.outputDir() + "/" + testCase.name() + ".out";

        try {
            applyInput(testCase);
            verifyInput(testCase);
            generateInput(testCase, inputFilename, options);
            generateAndApplyOutput(testCase, inputFilename, outputFilename, options);
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

    void generateInput(const TestCase& testCase, const string& inputFilename, const GenerationOptions& options) {
        ostream* input = os_->openForWriting(inputFilename);
        modifyInputForMultipleTestCases(input, options);

        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            SampleTestCaseData* data = (SampleTestCaseData*) testCase.data();
            *input << data->input();
        } else {
            ioManipulator_->printInput(input);
        }
        os_->closeOpenedStream(input);
    }

    void generateAndApplyOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GenerationOptions& options) {

        optional<string> maybeSampleOutputString = getSampleOutputString(testCase);
        if (!options.needsOutput()) {
            if (maybeSampleOutputString) {
                throw GenerationException([=] { logger_->logSampleTestCaseNoOutputNeededFailure(); });
            }
            return;
        }

        EvaluationOptions evaluationConfig = EvaluationOptionsBuilder()
                .setSolutionCommand(options.solutionCommand())
                .build();

        GenerationResult generationResult = evaluator_->generate(inputFilename, outputFilename, evaluationConfig);
        if (!generationResult.executionResult().isSuccessful()) {
            throw GenerationException([=] {
                logger_->logExecutionResults({{"solution", generationResult.executionResult()}});
            });
        }

        if (maybeSampleOutputString) {
            checkSampleOutput(maybeSampleOutputString.value(), inputFilename, outputFilename, options);
        }

        istream* output = os_->openForReading(outputFilename);
        modifyOutputForMultipleTestCases(output, options);
        ioManipulator_->parseOutput(output);
        os_->closeOpenedStream(output);
    }

    optional<string> getSampleOutputString(const TestCase& testCase) {
        if (testCase.data()->type() != TestCaseDataType::SAMPLE) {
            return optional<string>();
        }
        return ((SampleTestCaseData*) testCase.data())->output();
    }

    void modifyInputForMultipleTestCases(ostream* input, const GenerationOptions& options) {
        if (options.multipleTestCasesCounter() != nullptr) {
            int testCaseId = 1;
            *input << testCaseId << endl;
        }
    }

    void modifySampleOutputStringForMultipleTestCases(string& outputString, const GenerationOptions& options) {
        if (options.multipleTestCasesCounter() != nullptr && options.multipleTestCasesFirstOutputPrefix()) {
            outputString = options.multipleTestCasesFirstOutputPrefix().value() + outputString;
        }
    }

    void modifyOutputForMultipleTestCases(istream* output, const GenerationOptions& options) {
        if (options.multipleTestCasesCounter() != nullptr && options.multipleTestCasesFirstOutputPrefix()) {
            string prefix = options.multipleTestCasesOutputPrefix().value();
            string firstPrefix = options.multipleTestCasesFirstOutputPrefix().value();
            for (char p : firstPrefix) {
                int c = output->peek();
                if (c == char_traits<char>::eof() || (char) c != p) {
                    throw runtime_error("Output must start with \"" + prefix + "\"");
                }
                output->get();
            }
        }
    }

    void checkSampleOutput(
            const string& sampleOutputString,
            const string& inputFilename,
            const string& outputFilename,
            const GenerationOptions& options) {

        string modifiedSampleOutputString = sampleOutputString;
        modifySampleOutputStringForMultipleTestCases(modifiedSampleOutputString, options);

        ostream* sampleOutput = os_->openForWriting(Evaluator::EVALUATION_OUT_FILENAME);
        *sampleOutput << modifiedSampleOutputString;
        os_->closeOpenedStream(sampleOutput);

        ScoringResult scoringResult = evaluator_->score(inputFilename, outputFilename);
        if (!(scoringResult.verdict().status() == VerdictStatus::ac())) {
            throw GenerationException([=] {
                logger_->logSampleTestCaseCheckFailure();
                logger_->logExecutionResults({{"scorer", scoringResult.executionResult()}});
            });
        }
    }
};

}

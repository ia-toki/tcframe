#pragma once

#include <ostream>
#include <string>

#include "GenerationException.hpp"
#include "GenerationOptions.hpp"
#include "GeneratorLogger.hpp"
#include "tcframe/runner/client.hpp"
#include "tcframe/runner/evaluator.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/spec/testcase.hpp"

using std::ostream;
using std::string;

namespace tcframe {

class TestCaseGenerator {
private:
    SpecClient* specClient_;
    Evaluator* evaluator_;
    GeneratorLogger* logger_;

public:
    virtual ~TestCaseGenerator() {}

    TestCaseGenerator(
            SpecClient* specClient,
            Evaluator* evaluator,
            GeneratorLogger* logger)
            : specClient_(specClient)
            , evaluator_(evaluator)
            , logger_(logger) {}

    virtual bool generate(const TestCase& testCase, const GenerationOptions& options) {
        logger_->logTestCaseIntroduction(testCase.name());

        string inputFilename = TestCasePathCreator::createInputPath(testCase.name(), options.outputDir());
        string outputFilename = TestCasePathCreator::createOutputPath(testCase.name(), options.outputDir());

        try {
            generateInput(testCase, inputFilename);
            generateOutput(testCase, inputFilename, outputFilename, options);
            validateOutput(testCase, inputFilename, outputFilename, options);
        } catch (GenerationException& e) {
            logger_->logTestCaseFailedResult(testCase.description());
            e.callback()();
            return false;
        } catch (FormattedError& e) {
            logger_->logTestCaseFailedResult(testCase.description());
            logger_->logFormattedError(e);
            return false;
        } catch (runtime_error& e) {
            logger_->logTestCaseFailedResult(testCase.description());
            logger_->logSimpleError(e);
            return false;
        }

        logger_->logTestCaseSuccessfulResult();
        return true;
    }

private:
    void generateInput(const TestCase& testCase, const string& inputFilename) {
        specClient_->generateTestCaseInput(testCase.name(), inputFilename);
    }

    void generateOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GenerationOptions& options) {

        if (!options.needsOutput()) {
            return;
        }

        EvaluationOptions evaluationOptions = EvaluationOptionsBuilder()
                .setSolutionCommand(options.solutionCommand())
                .build();

        GenerationResult generationResult = evaluator_->generate(inputFilename, outputFilename, evaluationOptions);
        if (!generationResult.executionResult().isSuccessful()) {
            throw GenerationException([=] {
                logger_->logExecutionResults({{"solution", generationResult.executionResult()}});
            });
        }
    }

    void validateOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GenerationOptions& options) {

        validateSampleOutput(testCase, inputFilename, outputFilename, options);
        if (options.needsOutput()) {
            validateSolutionOutput(outputFilename);
        }
    }

    void validateSampleOutput(
            const TestCase& testCase,
            const string& inputFilename,
            const string& outputFilename,
            const GenerationOptions& options) {

        if (testCase.data()->type() != TestCaseDataType::SAMPLE) {
            return;
        }

        auto data = (SampleTestCaseData*) testCase.data();
        if (!data->output()) {
            return;
        }

        if (!options.needsOutput()) {
            throw runtime_error("Problem does not need test case outputs, but this sample test case has output");
        }

        specClient_->generateSampleTestCaseOutput(testCase.name(), Evaluator::EVALUATION_OUT_FILENAME);

        ScoringResult scoringResult = evaluator_->score(inputFilename, outputFilename);
        if (!(scoringResult.verdict().status() == VerdictStatus::ac())) {
            throw GenerationException([=] {
                logger_->logSimpleError(runtime_error(
                        "Sample test case output does not match with actual output produced by the solution"));
                logger_->logExecutionResults({{"scorer", scoringResult.executionResult()}});
            });
        }
    }

    void validateSolutionOutput(const string& outputFilename) {
        specClient_->validateTestCaseOutput(outputFilename);
    }
};

}

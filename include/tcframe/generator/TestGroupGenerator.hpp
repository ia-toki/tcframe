#pragma once

#include <string>
#include <vector>

#include "GeneratorLogger.hpp"
#include "TestCaseGenerator.hpp"
#include "TestGroupGenerationResult.hpp"
#include "MultipleTestCasesCombinationResult.hpp"
#include "tcframe/generator/failure.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"
#include "tcframe/verifier.hpp"

using std::string;
using std::vector;

namespace tcframe {

class TestGroupGenerator {
private:
    TestCaseGenerator* testCaseGenerator_;
    Verifier* verifier_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~TestGroupGenerator() {}

    TestGroupGenerator(
            TestCaseGenerator* testCaseGenerator,
            Verifier* verifier,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : testCaseGenerator_(testCaseGenerator)
            , verifier_(verifier)
            , os_(os)
            , logger_(logger)
    {}

    virtual TestGroupGenerationResult generate(const TestGroup& testGroup, const GeneratorConfig& config) {
        logger_->logTestGroupIntroduction(testGroup.id());

        vector<TestCaseGenerationResult> testCaseResults;
        bool successful = true;
        for (const TestCase& testCase : testGroup.testCases()) {
            TestCaseGenerationResult testCaseResult = testCaseGenerator_->generate(testCase, config);
            testCaseResults.push_back(testCaseResult);
            successful &= testCaseResult.isSuccessful();
        }

        Failure* failure = nullptr;

        if (successful &&
            config.multipleTestCasesCount() != nullptr && !testGroup.testCases().empty()) {

            string baseId = TestCaseIdCreator::createBaseId(config.slug(), testGroup.id());
            logger_->logMultipleTestCasesCombinationIntroduction(baseId);
            MultipleTestCasesCombinationResult combinationResult = combineMultipleTestCases(testGroup, config);
            logger_->logMultipleTestCasesCombinationResult(combinationResult);
            failure = combinationResult.failure();
        }

        return TestGroupGenerationResult(failure, testCaseResults);
    }

private:
    MultipleTestCasesCombinationResult combineMultipleTestCases(
            const TestGroup& testGroup,
            const GeneratorConfig& config) {

        *config.multipleTestCasesCount() = (int) testGroup.testCases().size();

        Failure* failure = nullptr;

        try {
            verify();
            combine(testGroup, config);
        } catch (ComplexFailureException& e) {
            failure = e.failure();
        } catch (runtime_error& e) {
            failure = new SimpleFailure(e.what());
        }
        return MultipleTestCasesCombinationResult(failure);
    }

    void verify() {
        MultipleTestCasesConstraintsVerificationResult result = verifier_->verifyMultipleTestCasesConstraints();
        if (!result.isValid()) {
            throw ComplexFailureException(new MultipleTestCasesConstraintsVerificationFailure(result));
        }
    }

    void combine(const TestGroup& testGroup, const GeneratorConfig& config) {
        string baseId = TestCaseIdCreator::createBaseId(config.slug(), testGroup.id());
        string baseFilename = config.testCasesDir() + "/" + baseId;
        os_->combineMultipleTestCases(baseFilename, (int) testGroup.testCases().size());
    }
};

}

#pragma once

#include <string>
#include <vector>

#include "GeneratorLogger.hpp"
#include "TestCase.hpp"
#include "TestCaseGenerator.hpp"
#include "TestCaseIdCreator.hpp"
#include "TestGroup.hpp"
#include "TestGroupGenerationResult.hpp"
#include "MultipleTestCasesCombinationResult.hpp"
#include "tcframe/core.hpp"
#include "tcframe/failure.hpp"
#include "tcframe/os.hpp"
#include "tcframe/verifier.hpp"

using std::string;
using std::vector;

namespace tcframe {

class TestGroupGenerator {
private:
    TestCaseGenerator* testCaseGenerator_;
    ConstraintSuiteVerifier* verifier_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~TestGroupGenerator() {}

    TestGroupGenerator(
            TestCaseGenerator* testCaseGenerator,
            ConstraintSuiteVerifier* verifier,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : testCaseGenerator_(testCaseGenerator)
            , verifier_(verifier)
            , os_(os)
            , logger_(logger)
    {}

    virtual TestGroupGenerationResult generate(const TestGroup& testGroup, const CoreConfig& coreConfig) {
        logger_->logTestGroupIntroduction(testGroup.id());

        vector<TestCaseGenerationResult> testCaseResults;
        bool successful = true;
        for (const TestCase& testCase : testGroup.testCases()) {
            TestCaseGenerationResult testCaseResult = testCaseGenerator_->generate(testCase, coreConfig);
            testCaseResults.push_back(testCaseResult);
            successful &= testCaseResult.isSuccessful();
        }

        Failure* failure = nullptr;

        if (successful &&
            coreConfig.problemConfig().multipleTestCasesCount() != nullptr && !testGroup.testCases().empty()) {

            string baseId = TestCaseIdCreator::createBaseId(coreConfig.problemConfig().slug(), testGroup.id());
            logger_->logMultipleTestCasesCombinationIntroduction(baseId);
            MultipleTestCasesCombinationResult combinationResult = combineMultipleTestCases(testGroup, coreConfig);
            logger_->logMultipleTestCasesCombinationResult(combinationResult);
            failure = combinationResult.failure();
        }

        return TestGroupGenerationResult(failure, testCaseResults);
    }

private:
    MultipleTestCasesCombinationResult combineMultipleTestCases(
            const TestGroup& testGroup,
            const CoreConfig& coreConfig) {

        *coreConfig.problemConfig().multipleTestCasesCount() = (int) testGroup.testCases().size();

        Failure* failure = nullptr;

        try {
            verify();
            combine(testGroup, coreConfig);
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

    void combine(const TestGroup& testGroup, const CoreConfig& coreConfig) {
        string baseId = TestCaseIdCreator::createBaseId(coreConfig.problemConfig().slug(), testGroup.id());
        string baseFilename = coreConfig.testConfig().testCasesDir() + "/" + baseId;
        os_->combineMultipleTestCases(baseFilename, (int) testGroup.testCases().size());
    }
};

}

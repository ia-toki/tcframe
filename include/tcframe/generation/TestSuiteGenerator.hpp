#pragma once

#include <functional>
#include <map>
#include <set>
#include <string>

#include "TestCaseNameCreator.hpp"
#include "TestCaseGenerationResult.hpp"
#include "TestCaseGenerator.hpp"
#include "TestSuiteGenerationListener.hpp"
#include "TestSuiteGenerationResult.hpp"
#include "tcframe/config.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"

using std::function;
using std::map;
using std::set;
using std::string;

namespace tcframe {

class TestSuiteGenerator {
private:
    TestCaseGenerator* testCaseGenerator_;
    OperatingSystem* os_;
    TestSuiteGenerationListener* listener_;

public:
    virtual ~TestSuiteGenerator() {}

    TestSuiteGenerator(
            TestCaseGenerator* testCaseGenerator,
            OperatingSystem* os,
            TestSuiteGenerationListener* listener)
            : testCaseGenerator_(testCaseGenerator)
            , os_(os)
            , listener_(listener)
    {}

    TestSuiteGenerationResult generate(
            const TestSuite& testSuite,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig) {

        listener_->onIntroduction();

        os_->forceMakeDir(generatorConfig.testCasesDir());

        map<string, TestCaseGenerationResult> resultsByTestCaseName;
        generateOfficialTests(testSuite.officialTests(), problemConfig, generatorConfig, resultsByTestCaseName);
        return TestSuiteGenerationResult(resultsByTestCaseName);
    }

private:
    void generateOfficialTests(
            const vector<TestGroup> &testCases,
            const ProblemConfig &problemConfig,
            const GeneratorConfig &generatorConfig,
            map<string, TestCaseGenerationResult> &testCaseGenerationResultByTestCaseName) {

        for (const TestGroup& testGroup : testCases) {
            generateTestGroup(testGroup, problemConfig, generatorConfig, testCaseGenerationResultByTestCaseName);
        }
    }

    void generateTestGroup(
            const TestGroup& testGroup,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& testCaseGenerationResultByTestCaseName) {

        listener_->onTestGroupIntroduction(testGroup.id());

        for (int testCaseId = 1; testCaseId <= testGroup.officialTestCases().size(); testCaseId++) {
            OfficialTestCase testCase = testGroup.officialTestCases()[testCaseId - 1];
            TestCaseData testCaseData = TestCaseDataBuilder()
                    .setName(TestCaseNameCreator::createTestCaseName(problemConfig.slug(), testGroup.id(), testCaseId))
                    .setDescription(testCase.description())
                    .setConstraintGroupIds(testGroup.constraintGroupIds())
                    .build();

            generateTestCase(testCaseData, testCase.closure(), generatorConfig, testCaseGenerationResultByTestCaseName);
        }
    }

    TestCaseGenerationResult generateTestCase(
            const TestCaseData& testCaseData,
            const function<void()> testCaseClosure,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& testCaseGenerationResultsByName) {

        listener_->onTestCaseIntroduction(testCaseData.name());

        TestCaseGenerationResult result = testCaseGenerator_->generate(testCaseData, testCaseClosure, generatorConfig);
        testCaseGenerationResultsByName[testCaseData.name()] = result;

        listener_->onTestCaseGenerationResult(testCaseData.description(), result);

        return result;
    }
};

}

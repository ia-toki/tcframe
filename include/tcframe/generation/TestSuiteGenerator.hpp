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

        map<string, TestCaseGenerationResult> resultsByName;
        generateSampleTests(testSuite.sampleTests(), problemConfig, generatorConfig, resultsByName);
        generateOfficialTests(testSuite.officialTests(), problemConfig, generatorConfig, resultsByName);
        return TestSuiteGenerationResult(resultsByName);
    }

private:
    void generateSampleTests(
            const vector<SampleTestCase>& sampleTests,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& resultsByName) {

        listener_->onSampleTestCasesIntroduction();

        for (int testCaseId = 1; testCaseId <= sampleTests.size(); testCaseId++) {
            SampleTestCase testCase = sampleTests[testCaseId - 1];
            TestCaseData testCaseData = TestCaseDataBuilder()
                    .setName(TestCaseNameCreator::createSampleTestCaseName(problemConfig.slug(), testCaseId))
                    .setConstraintGroupIds(testCase.constraintGroupIds())
                    .build();

            generateTestCase(testCaseData, []{}, generatorConfig, resultsByName);
        }
    }

    void generateOfficialTests(
            const vector<TestGroup>& officialTests,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& resultsByName) {

        for (const TestGroup& testGroup : officialTests) {
            generateTestGroup(testGroup, problemConfig, generatorConfig, resultsByName);
        }
    }

    void generateTestGroup(
            const TestGroup& testGroup,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& resultsByName) {

        listener_->onTestGroupIntroduction(testGroup.id());

        for (int testCaseId = 1; testCaseId <= testGroup.officialTestCases().size(); testCaseId++) {
            OfficialTestCase testCase = testGroup.officialTestCases()[testCaseId - 1];
            TestCaseData testCaseData = TestCaseDataBuilder()
                    .setName(TestCaseNameCreator::createOfficialTestCaseName(problemConfig.slug(), testGroup.id(), testCaseId))
                    .setDescription(testCase.description())
                    .setConstraintGroupIds(testGroup.constraintGroupIds())
                    .build();

            generateTestCase(testCaseData, testCase.closure(), generatorConfig, resultsByName);
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

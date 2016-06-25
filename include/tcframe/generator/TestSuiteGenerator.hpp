#pragma once

#include <functional>
#include <vector>
#include <set>
#include <sstream>
#include <string>

#include "GeneratorLogger.hpp"
#include "GenerationResult.hpp"
#include "TestCase.hpp"
#include "TestGroup.hpp"
#include "TestCaseNameCreator.hpp"
#include "TestGroupGenerationResult.hpp"
#include "TestGroupGenerator.hpp"
#include "tcframe/core.hpp"
#include "tcframe/io.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"

using std::function;
using std::istringstream;
using std::vector;
using std::set;
using std::string;

namespace tcframe {

class TestSuiteGenerator {
private:
    TestGroupGenerator* testGroupGenerator_;
    IOManipulator* ioManipulator_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~TestSuiteGenerator() {}

    TestSuiteGenerator(
            TestGroupGenerator* testGroupGenerator,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : testGroupGenerator_(testGroupGenerator)
            , ioManipulator_(ioManipulator)
            , os_(os)
            , logger_(logger)
    {}

    virtual GenerationResult generate(const TestSuite& testSuite, const CoreConfig& coreConfig) {
        logger_->logIntroduction();

        os_->forceMakeDir(coreConfig.testConfig().testCasesDir());

        vector<TestGroupGenerationResult> testGroupResults;
        testGroupResults.push_back(generateSampleTests(testSuite, coreConfig));
        for (TestGroupGenerationResult result : generateOfficialTests(testSuite, coreConfig)) {
            testGroupResults.push_back(result);
        }

        return GenerationResult(testGroupResults);
    }

private:
    TestGroupGenerationResult generateSampleTests(const TestSuite& testSuite, const CoreConfig& coreConfig) {
        logger_->logTestGroupIntroduction(0);

        vector<SampleTestCase> sampleTests = testSuite.sampleTests();
        vector<TestCase> testCases;
        for (int testCaseId = 1; testCaseId <= sampleTests.size(); testCaseId++) {
            SampleTestCase sampleTestCase = sampleTests[testCaseId - 1];
            TestCase testCase = TestCaseBuilder()
                    .setName(TestCaseNameCreator::createSampleTestCaseName(coreConfig.problemConfig().slug(), testCaseId))
                    .setSubtaskIds(sampleTestCase.subtaskIds())
                    .setApplier([=] {
                        istream* in = new istringstream(sampleTestCase.content());
                        ioManipulator_->parseInput(in);
                    })
                    .build();
            testCases.push_back(testCase);
        }

        return testGroupGenerator_->generate(TestGroup(0, testCases), coreConfig.testConfig());
    }

    vector<TestGroupGenerationResult> generateOfficialTests(const TestSuite& testSuite, const CoreConfig& coreConfig) {
        vector<TestGroupGenerationResult> results;
        for (const OfficialTestGroup& officialTestGroup : testSuite.officialTests()) {
            results.push_back(generateOfficialTestGroup(officialTestGroup, testSuite.inputFinalizer(), coreConfig));
        }
        return results;
    }

    TestGroupGenerationResult generateOfficialTestGroup(
            const OfficialTestGroup& officialTestGroup,
            const function<void()>& inputFinalizer,
            const CoreConfig& coreConfig) {

        logger_->logTestGroupIntroduction(officialTestGroup.id());

        vector<OfficialTestCase> officialTestCases = officialTestGroup.officialTestCases();
        vector<TestCase> testCases;
        for (int testCaseId = 1; testCaseId <= officialTestCases.size(); testCaseId++) {
            OfficialTestCase officialTestCase = officialTestCases[testCaseId - 1];
            string testCaseName = TestCaseNameCreator::createOfficialTestCaseName(
                    coreConfig.problemConfig().slug(),
                    officialTestGroup.id(),
                    testCaseId);
            TestCase testCase = TestCaseBuilder()
                    .setName(testCaseName)
                    .setDescription(officialTestCase.description())
                    .setSubtaskIds(officialTestGroup.subtaskIds())
                    .setApplier([=] {officialTestCase.closure()(); inputFinalizer();})
                    .build();
            testCases.push_back(testCase);
        }

        return testGroupGenerator_->generate(TestGroup(officialTestGroup.id(), testCases), coreConfig.testConfig());
    }
};

class TestSuiteGeneratorFactory {
public:
    virtual ~TestSuiteGeneratorFactory() {}

    virtual TestSuiteGenerator* create(
            TestGroupGenerator* testGroupGenerator,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GeneratorLogger* logger) {

        return new TestSuiteGenerator(testGroupGenerator, ioManipulator, os, logger);
    }
};

}

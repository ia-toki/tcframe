#pragma once

#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "GenerationLogger.hpp"
#include "GenerationResult.hpp"
#include "TestCaseNameCreator.hpp"
#include "TestCaseGenerationResult.hpp"
#include "TestCaseGenerator.hpp"
#include "tcframe/core.hpp"
#include "tcframe/io.hpp"
#include "tcframe/os.hpp"
#include "tcframe/testcase.hpp"

using std::function;
using std::istringstream;
using std::map;
using std::set;
using std::string;

namespace tcframe {

class Generator {
private:
    TestCaseGenerator* testCaseGenerator_;
    IOManipulator* ioManipulator_;
    OperatingSystem* os_;
    GenerationLogger* logger_;

public:
    virtual ~Generator() {}

    Generator(
            TestCaseGenerator* testCaseGenerator,
            IOManipulator* ioManipulator,
            OperatingSystem* os,
            GenerationLogger* logger)
            : testCaseGenerator_(testCaseGenerator)
            , ioManipulator_(ioManipulator)
            , os_(os)
            , logger_(logger)
    {}

    GenerationResult generate(
            const TestSuite& testSuite,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig) {

        logger_->logIntroduction();

        os_->forceMakeDir(generatorConfig.testCasesDir());

        map<string, TestCaseGenerationResult> resultsByName;
        generateSampleTests(testSuite.sampleTests(), problemConfig, generatorConfig, resultsByName);
        generateOfficialTests(testSuite.officialTests(), problemConfig, generatorConfig, resultsByName);
        return GenerationResult(resultsByName);
    }

private:
    void generateSampleTests(
            const vector<SampleTestCase>& sampleTests,
            const ProblemConfig& problemConfig,
            const GeneratorConfig& generatorConfig,
            map<string, TestCaseGenerationResult>& resultsByName) {

        logger_->logSampleTestCasesIntroduction();

        for (int testCaseId = 1; testCaseId <= sampleTests.size(); testCaseId++) {
            SampleTestCase testCase = sampleTests[testCaseId - 1];
            TestCaseData testCaseData = TestCaseDataBuilder()
                    .setName(TestCaseNameCreator::createSampleTestCaseName(problemConfig.slug(), testCaseId))
                    .setConstraintGroupIds(testCase.constraintGroupIds())
                    .build();

            function<void()> closure = [=] {
                istream* in = new istringstream(testCase.content());
                ioManipulator_->parseInput(in);
            };

            generateTestCase(testCaseData, closure, generatorConfig, resultsByName);
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

        logger_->logTestGroupIntroduction(testGroup.id());

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

        logger_->logTestCaseIntroduction(testCaseData.name());

        TestCaseGenerationResult result = testCaseGenerator_->generate(testCaseData, testCaseClosure, generatorConfig);
        testCaseGenerationResultsByName[testCaseData.name()] = result;

        logger_->logTestCaseGenerationResult(testCaseData.description(), result);

        return result;
    }
};

}

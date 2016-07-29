#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "TestCaseIdCreator.hpp"
#include "TestGroup.hpp"
#include "tcframe/io_manipulator.hpp"
#include "tcframe/spec/raw_testcase.hpp"
#include "tcframe/util.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct TestSuite {
private:
    vector<TestGroup> testGroups_;

public:
    TestSuite(const vector<TestGroup>& testGroups)
            : testGroups_(testGroups) {}

    const vector<TestGroup>& testGroups() const {
        return testGroups_;
    }

    bool operator==(const TestSuite& o) const {
        return tie(testGroups_) == tie(testGroups_);
    }
};

class TestSuiteProvider {
public:
    static TestSuite provide(
            const RawTestSuite& rawTestSuite,
            const string& slug,
            const optional<IOManipulator*>& ioManipulator) {

        vector<TestGroup> testGroups;
        testGroups.push_back(provideSampleTests(rawTestSuite, slug, ioManipulator));
        for (TestGroup testGroup : provideOfficialTests(rawTestSuite, slug)) {
            testGroups.push_back(testGroup);
        }
        return TestSuite(testGroups);
    }

private:
    static TestGroup provideSampleTests(
            const RawTestSuite& rawTestSuite,
            const string& slug,
            const optional<IOManipulator*>& ioManipulator) {

        vector<SampleTestCase> sampleTests = rawTestSuite.sampleTests();
        vector<TestCase> testCases;
        for (int testCaseNo = 1; testCaseNo <= sampleTests.size(); testCaseNo++) {
            SampleTestCase sampleTestCase = sampleTests[testCaseNo - 1];
            TestCaseBuilder builder = TestCaseBuilder()
                    .setId(TestCaseIdCreator::create(slug, 0, testCaseNo))
                    .setSubtaskIds(sampleTestCase.subtaskIds());
            if (ioManipulator) {
                builder.setApplier([=] {
                    istream* in = new istringstream(sampleTestCase.content());
                    ioManipulator.value()->parseInput(in);
                });
            }
            testCases.push_back(builder.build());
        }

        return TestGroup(0, testCases);
    }

    static vector<TestGroup> provideOfficialTests(const RawTestSuite& rawTestSuite, const string& slug) {
        vector<TestGroup> testGroups;
        for (const OfficialTestGroup& officialTestGroup : rawTestSuite.officialTests()) {
            testGroups.push_back(provideOfficialTestGroup(officialTestGroup, rawTestSuite.inputFinalizer(), slug));
        }
        return testGroups;
    }

    static TestGroup provideOfficialTestGroup(
            const OfficialTestGroup& officialTestGroup,
            const function<void()>& inputFinalizer,
            const string& slug) {

        vector<OfficialTestCase> officialTestCases = officialTestGroup.officialTestCases();
        vector<TestCase> testCases;
        for (int testCaseNo = 1; testCaseNo <= officialTestCases.size(); testCaseNo++) {
            OfficialTestCase officialTestCase = officialTestCases[testCaseNo - 1];
            string testCaseId = TestCaseIdCreator::create(
                    slug,
                    officialTestGroup.id(),
                    testCaseNo);
            TestCase testCase = TestCaseBuilder()
                    .setId(testCaseId)
                    .setDescription(officialTestCase.description())
                    .setSubtaskIds(officialTestGroup.subtaskIds())
                    .setApplier([=] {officialTestCase.closure()(); inputFinalizer();})
                    .build();
            testCases.push_back(testCase);
        }

        return TestGroup(officialTestGroup.id(), testCases);
    }
};

}

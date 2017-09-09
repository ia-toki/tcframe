#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../util/TestUtils.hpp"
#include "MockTestCaseDriver.hpp"
#include "tcframe/driver/SpecDriver.hpp"

using ::testing::Eq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class SpecDriverTests : public Test {
protected:
    MOCK(TestCaseDriver) testCaseDriver;

    TestCase stc = TestUtils::newSampleTestCase("foo_sample_1");
    TestCase tc1 = TestUtils::newTestCase("foo_1");
    TestCase tc2 = TestUtils::newTestCase("foo_2");

    TestSuite testSuite = TestSuite({TestGroup(0, {stc}), TestGroup(-1, {tc1, tc2})});

    SpecDriver driver = SpecDriver(&testCaseDriver, testSuite);
};

TEST_F(SpecDriverTests, GetTestSuite) {
    EXPECT_THAT(driver.getTestSuite(), Eq(testSuite));
}

TEST_F(SpecDriverTests, GenerateTestCaseInput) {
    ostringstream out;
    EXPECT_CALL(testCaseDriver, generateInput(tc2, &out));
    driver.generateTestCaseInput("foo_2", &out);
}

TEST_F(SpecDriverTests, GenerateSampleTestCaseOutput) {
    ostringstream out;
    EXPECT_CALL(testCaseDriver, generateSampleOutput(stc, &out));
    driver.generateSampleTestCaseOutput("foo_sample_1", &out);
}

TEST_F(SpecDriverTests, ValidateTestCaseOutput) {
    istringstream in;
    EXPECT_CALL(testCaseDriver, validateOutput(&in));
    driver.validateTestCaseOutput(&in);
}

TEST_F(SpecDriverTests, ValidateMultipleTestCasesInput) {
    EXPECT_CALL(testCaseDriver, validateMultipleTestCasesInput(3));
    driver.validateMultipleTestCasesInput(3);
}

}

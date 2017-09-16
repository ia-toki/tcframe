#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>

#include "../../util/TestUtils.hpp"
#include "../../driver/MockSpecDriver.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/client/SpecClient.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;

namespace tcframe {

class SpecClientTests : public Test {
protected:
    MOCK(SpecDriver) specDriver;
    MOCK(OperatingSystem) os;

    istream* in = new istringstream();
    ostream* out = new ostringstream();

    SpecClient client = {&specDriver, &os};

    void SetUp() {
        ON_CALL(os, openForReading(_))
                .WillByDefault(Return(in));
        ON_CALL(os, openForWriting(_))
                .WillByDefault(Return(out));
    }
};

TEST_F(SpecClientTests, GetTestSuite) {
    TestSuite testSuite({TestGroup(-1, {})});
    ON_CALL(specDriver, getTestSuite())
            .WillByDefault(Return(testSuite));
    EXPECT_THAT(client.getTestSuite(), Eq(testSuite));
}

TEST_F(SpecClientTests, SetSeed) {
    EXPECT_CALL(specDriver, setSeed(42));
    client.setSeed(42);
}

TEST_F(SpecClientTests, HasMultipleTestCases) {
    ON_CALL(specDriver, hasMultipleTestCases())
            .WillByDefault(Return(true));
    EXPECT_TRUE(client.hasMultipleTestCases());
}

TEST_F(SpecClientTests, GetMultipleTestCasesOutputPrefix) {
    ON_CALL(specDriver, getMultipleTestCasesOutputPrefix())
            .WillByDefault(Return(optional<string>("Case #%d: ")));
    EXPECT_THAT(client.getMultipleTestCasesOutputPrefix(), Eq(optional<string>("Case #%d: ")));
}

TEST_F(SpecClientTests, GenerateTestCaseInput) {
    {
        InSequence sequence;
        EXPECT_CALL(os, openForWriting("foo_1.out"));
        EXPECT_CALL(specDriver, generateTestCaseInput("foo_1", out));
        EXPECT_CALL(os, closeOpenedStream(out));
    }
    client.generateTestCaseInput("foo_1", "foo_1.out");
}

TEST_F(SpecClientTests, GenerateSampleTestCaseOutput) {
    {
        InSequence sequence;
        EXPECT_CALL(os, openForWriting("foo_sample_1.out"));
        EXPECT_CALL(specDriver, generateSampleTestCaseOutput("foo_sample_1", out));
        EXPECT_CALL(os, closeOpenedStream(out));
    }
    client.generateSampleTestCaseOutput("foo_sample_1", "foo_sample_1.out");
}

TEST_F(SpecClientTests, ValidateTestCaseOutput) {
    {
        InSequence sequence;
        EXPECT_CALL(os, openForReading("foo_1.out"));
        EXPECT_CALL(specDriver, validateTestCaseOutput(in));
        EXPECT_CALL(os, closeOpenedStream(in));

    }
    client.validateTestCaseOutput("foo_1.out");
}

TEST_F(SpecClientTests, ValidateMultipleTestCasesInput) {
    EXPECT_CALL(specDriver, validateMultipleTestCasesInput(3));
    client.validateMultipleTestCasesInput(3);
}

}

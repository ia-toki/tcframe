#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>
#include <streambuf>

#include "../spec/io/MockIOManipulator.hpp"
#include "../spec/verifier/MockVerifier.hpp"
#include "tcframe/driver/TestCaseDriver.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Test;
using ::testing::Truly;

using std::istreambuf_iterator;
using std::istringstream;
using std::ostringstream;

namespace tcframe {

class TestCaseDriverTests : public Test {
public:
    static int T;
    static int N;

protected:
    MOCK(IOManipulator) ioManipulator;
    MOCK(Verifier) verifier;

    TestCase sampleTestCase = TestCaseBuilder()
            .setName("foo_sample_1")
            .setSubtaskIds({1, 2})
            .setData(new SampleTestCaseData("42\n", "yes\n"))
            .build();
    TestCase officialTestCase = TestCaseBuilder()
            .setName("foo_1")
            .setDescription("N = 42")
            .setSubtaskIds({1, 2})
            .setData(new OfficialTestCaseData([&]{N = 42;}))
            .build();
    MultipleTestCasesConfig multipleTestCasesConfig = MultipleTestCasesConfigBuilder()
            .Counter(T)
            .build();
    MultipleTestCasesConfig multipleTestCasesConfigWithOutputPrefix = MultipleTestCasesConfigBuilder()
            .Counter(T)
            .OutputPrefix("Case #%d: ")
            .build();

    TestCaseDriver driver = createDriver(MultipleTestCasesConfig());
    TestCaseDriver driverWithMultipleTestCases = createDriver(multipleTestCasesConfig);
    TestCaseDriver driverWithMultipleTestCasesWithOutputPrefix = createDriver(multipleTestCasesConfigWithOutputPrefix);

    TestCaseDriver createDriver(const MultipleTestCasesConfig& multipleTestCasesConfig) {
        return {&ioManipulator, &verifier, multipleTestCasesConfig};
    }

    void SetUp() {
        ON_CALL(verifier, verifyConstraints(_))
                .WillByDefault(Return(ConstraintsVerificationResult()));
        ON_CALL(verifier, verifyMultipleTestCasesConstraints())
                .WillByDefault(Return(MultipleTestCasesConstraintsVerificationResult()));
    }

    struct InputStreamContentIs {
        string content_;

        explicit InputStreamContentIs(const string& content)
                : content_(content) {}

        bool operator()(istream* in) const {
            return content_ == string(istreambuf_iterator<char>(*in), istreambuf_iterator<char>());
        }
    };
};

int TestCaseDriverTests::T;
int TestCaseDriverTests::N;

TEST_F(TestCaseDriverTests, GenerateInput_Sample) {
    ostringstream out;
    {
        InSequence sequence;
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
    }
    driver.generateInput(sampleTestCase, &out);
    EXPECT_THAT(out.str(), Eq("42\n"));
}

TEST_F(TestCaseDriverTests, GenerateInput_Official) {
    ostringstream out;
    {
        InSequence sequence;
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(ioManipulator, printInput(&out));
    }
    N = 0;
    driver.generateInput(officialTestCase, &out);
    EXPECT_THAT(N, Eq(42));
}

TEST_F(TestCaseDriverTests, GenerateInput_Failed_Verification) {
    ConstraintsVerificationResult failedResult({}, {1});
    ON_CALL(verifier, verifyConstraints(_))
            .WillByDefault(Return(failedResult));
    ostringstream out;
    try {
        driver.generateInput(officialTestCase, &out);
        FAIL();
    } catch (FormattedError& e) {
        EXPECT_THAT(e, Eq(failedResult.asFormattedError()));
    }
}

TEST_F(TestCaseDriverTests, GenerateSampleOutput) {
    ostringstream out;
    EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
    driver.generateSampleOutput(sampleTestCase, &out);
    EXPECT_THAT(out.str(), Eq("yes\n"));
}

TEST_F(TestCaseDriverTests, ValidateOutput) {
    istringstream in;
    EXPECT_CALL(ioManipulator, parseOutput(&in));
    driver.validateOutput(&in);
}

TEST_F(TestCaseDriverTests, GenerateInput_MultipleTestCases_Sample) {
    ostringstream out;
    {
        InSequence sequence;
        EXPECT_CALL(ioManipulator, parseInput(Truly(InputStreamContentIs("42\n"))));
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
    }
    driverWithMultipleTestCases.generateInput(sampleTestCase, &out);
    EXPECT_THAT(out.str(), Eq("1\n42\n"));
}

TEST_F(TestCaseDriverTests, GenerateInput_MultipleTestCases_Official) {
    ostringstream out;
    {
        InSequence sequence;
        EXPECT_CALL(verifier, verifyConstraints(set<int>{1, 2}));
        EXPECT_CALL(ioManipulator, printInput(&out));
    }
    N = 0;
    driverWithMultipleTestCases.generateInput(officialTestCase, &out);
    EXPECT_THAT(N, Eq(42));
    EXPECT_THAT(out.str(), Eq("1\n"));
}

TEST_F(TestCaseDriverTests, GenerateSampleOutput_MultipleTestCases) {
    ostringstream out;
    EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
    driverWithMultipleTestCases.generateSampleOutput(sampleTestCase, &out);
    EXPECT_THAT(out.str(), Eq("yes\n"));
}

TEST_F(TestCaseDriverTests, ValidateOutput_MultipleTestCases) {
    istringstream in("yes\n");
    EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
    driver.validateOutput(&in);
}

TEST_F(TestCaseDriverTests, GenerateSampleOutput_MultipleTestCases_WithOutputPrefix) {
    ostringstream out;
    EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
    driverWithMultipleTestCasesWithOutputPrefix.generateSampleOutput(sampleTestCase, &out);
    EXPECT_THAT(out.str(), Eq("Case #1: yes\n"));
}

TEST_F(TestCaseDriverTests, ValidateOutput_MultipleTestCases_WithOutputPrefix) {
    istringstream in("Case #1: yes\n");
    EXPECT_CALL(ioManipulator, parseOutput(Truly(InputStreamContentIs("yes\n"))));
    driverWithMultipleTestCasesWithOutputPrefix.validateOutput(&in);
}

TEST_F(TestCaseDriverTests, ValidateOutput_MultipleTestCases_WithOutputPrefix_Failed) {
    istringstream in("yes\n");
    try {
        driverWithMultipleTestCasesWithOutputPrefix.validateOutput(&in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Output must start with \"Case #1: \""));
    }
}

TEST_F(TestCaseDriverTests, ValidateMultipleTestCasesInput) {
    EXPECT_CALL(verifier, verifyMultipleTestCasesConstraints());
    driverWithMultipleTestCases.validateMultipleTestCasesInput(3);
    EXPECT_THAT(T, Eq(3));
}

TEST_F(TestCaseDriverTests, ValidateMultipleTestCasesInput_Failed) {
    MultipleTestCasesConstraintsVerificationResult failedResult({"desc"});
    ON_CALL(verifier, verifyMultipleTestCasesConstraints())
            .WillByDefault(Return(failedResult));
    try {
        driverWithMultipleTestCases.validateMultipleTestCasesInput(3);
        FAIL();
    } catch (FormattedError& e) {
        EXPECT_THAT(e, Eq(failedResult.asFormattedError()));
    }
}

}

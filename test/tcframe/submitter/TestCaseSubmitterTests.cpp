#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockEvaluator.hpp"
#include "MockScorer.hpp"
#include "MockSubmitterLogger.hpp"
#include "tcframe/submitter/Submitter.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class TestCaseSubmitterTests : public Test {
protected:
    MOCK(Evaluator) evaluator;
    MOCK(Scorer) scorer;
    MOCK(SubmitterLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    SubmitterConfig config = SubmitterConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();

    TestCaseSubmitter submitter = TestCaseSubmitter(&evaluator, &scorer, &logger);

    void SetUp() {
        ON_CALL(evaluator, evaluate(_, _))
                .WillByDefault(Return(optional<Verdict>()));
        ON_CALL(scorer, score(_, _))
                .WillByDefault(Return(Verdict::ac()));
    }
};

TEST_F(TestCaseSubmitterTests, Submission_AC) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
        EXPECT_CALL(scorer, score(testCase, config));
    }
    EXPECT_THAT(submitter.submit(testCase, config), Eq(Verdict::ac()));
}

TEST_F(TestCaseSubmitterTests, Submission_WA) {
    ON_CALL(scorer, score(testCase, _))
            .WillByDefault(Return(Verdict::wa()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
        EXPECT_CALL(scorer, score(testCase, config));
    }
    EXPECT_THAT(submitter.submit(testCase, config), Eq(Verdict::wa()));
}

TEST_F(TestCaseSubmitterTests, Submission_RTE) {
    ON_CALL(evaluator, evaluate(_, _))
            .WillByDefault(Return(optional<Verdict>(Verdict::rte())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
    }
    EXPECT_THAT(submitter.submit(testCase, config), Eq(Verdict::rte()));
}

}

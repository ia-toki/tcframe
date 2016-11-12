#include "gmock/gmock.h"
#include "../mock.hpp"

#include "MockEvaluator.hpp"
#include "MockGraderLogger.hpp"
#include "MockScorer.hpp"
#include "tcframe/grader/Grader.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class TestCaseGraderTests : public Test {
protected:
    MOCK(Evaluator) evaluator;
    MOCK(Scorer) scorer;
    MOCK(GraderLogger) logger;

    TestCase testCase = TestCaseBuilder().setId("foo_1").build();

    GraderConfig config = GraderConfigBuilder("foo")
            .setSolutionCommand("python Sol.py")
            .setOutputDir("dir")
            .build();

    TestCaseGrader grader = TestCaseGrader(&evaluator, &scorer, &logger);

    void SetUp() {
        ON_CALL(evaluator, evaluate(_, _))
                .WillByDefault(Return(optional<Verdict>()));
        ON_CALL(scorer, score(_, _))
                .WillByDefault(Return(Verdict::ac()));
    }
};

TEST_F(TestCaseGraderTests, Grading_AC) {
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
        EXPECT_CALL(scorer, score(testCase, config));
    }
    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::ac()));
}

TEST_F(TestCaseGraderTests, Grading_WA) {
    ON_CALL(scorer, score(testCase, _))
            .WillByDefault(Return(Verdict::wa()));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
        EXPECT_CALL(scorer, score(testCase, config));
    }
    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::wa()));
}

TEST_F(TestCaseGraderTests, Grading_RTE) {
    ON_CALL(evaluator, evaluate(_, _))
            .WillByDefault(Return(optional<Verdict>(Verdict::rte())));
    {
        InSequence sequence;
        EXPECT_CALL(logger, logTestCaseIntroduction("foo_1"));
        EXPECT_CALL(evaluator, evaluate(testCase, config));
    }
    EXPECT_THAT(grader.grade(testCase, config), Eq(Verdict::rte()));
}

}

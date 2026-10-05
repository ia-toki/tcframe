#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/ThresholdAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class ThresholdAggregatorTests : public Test {
protected:
    ThresholdAggregator aggregator = ThresholdAggregator(18);
};

TEST_F(ThresholdAggregatorTests, Aggregate_AllAC_FullPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ac())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::ac(), 25)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_NoTestCases_FullPoints) {
    EXPECT_THAT(aggregator.aggregate({}, 25), Eq(SubtaskVerdict(Verdict::ac(), 25)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_OKWithinThreshold_FullPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ok(), 10),
            TestCaseVerdict(Verdict::ok(), 18)};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::ac(), 25)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_OKAboveThreshold_WAZeroPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ok(), 19)};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::wa(), 0)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_OKWithoutValue_WAZeroPoints) {
    vector<TestCaseVerdict> verdicts = {TestCaseVerdict(Verdict::ok())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::wa(), 0)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_OKPercentageOnly_WAZeroPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ok(), optional<double>(), optional<double>(10))};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::wa(), 0)));
}

TEST_F(ThresholdAggregatorTests, Aggregate_WorstFailingVerdict_ZeroPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::wa()),
            TestCaseVerdict(Verdict::ok(), 30),
            TestCaseVerdict(Verdict::tle())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 25), Eq(SubtaskVerdict(Verdict::tle(), 0)));
}

}

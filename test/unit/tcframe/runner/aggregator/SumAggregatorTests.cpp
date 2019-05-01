#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/SumAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SumAggregatorTests : public Test {
protected:
    SumAggregator aggregator;
};

TEST_F(SumAggregatorTests, Aggregate_FullPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ac())};
    EXPECT_THAT(aggregator.aggregate(verdicts, 100), Eq(TestCaseVerdict(Verdict::ac(), 100)));
}

TEST_F(SumAggregatorTests, Aggregate_PartialPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::tle()),
            TestCaseVerdict(Verdict::ok(), 30),
            TestCaseVerdict(Verdict::wa())};
    EXPECT_THAT(aggregator.aggregate(verdicts, 100), Eq(TestCaseVerdict(Verdict::tle(), 55)));
}

TEST_F(SumAggregatorTests, Aggregate_EmptyVerdicts) {
    EXPECT_THAT(aggregator.aggregate(vector<TestCaseVerdict>(), 100), Eq(TestCaseVerdict(Verdict::ac(), 100)));
}

}

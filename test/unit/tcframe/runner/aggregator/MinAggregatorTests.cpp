#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/MinAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class MinAggregatorTests : public Test {
protected:
    MinAggregator aggregator;
};

TEST_F(MinAggregatorTests, Aggregate_FullPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ac())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(TestCaseVerdict(Verdict::ac(), 70)));
}

TEST_F(MinAggregatorTests, Aggregate_ZeroPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::rte()),
            TestCaseVerdict(Verdict::wa())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(TestCaseVerdict(Verdict::rte(), 0)));
}

TEST_F(MinAggregatorTests, Aggregate_MinOKPoints) {
    vector<TestCaseVerdict> verdicts = {
            TestCaseVerdict(Verdict::ac()),
            TestCaseVerdict(Verdict::ok(), 20),
            TestCaseVerdict(Verdict::ok(), 30)};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(TestCaseVerdict(Verdict::ok(), 20)));
}

}

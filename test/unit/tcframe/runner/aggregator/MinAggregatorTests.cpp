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
    vector<Verdict> verdicts = {
            Verdict(VerdictStatus::ac()),
            Verdict(VerdictStatus::ac())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(Verdict(VerdictStatus::ac(), 70)));
}

TEST_F(MinAggregatorTests, Aggregate_ZeroPoints) {
    vector<Verdict> verdicts = {
            Verdict(VerdictStatus::ac()),
            Verdict(VerdictStatus::rte()),
            Verdict(VerdictStatus::wa())};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(Verdict(VerdictStatus::rte(), 0)));
}

TEST_F(MinAggregatorTests, Aggregate_MinOKPoints) {
    vector<Verdict> verdicts = {
            Verdict(VerdictStatus::ac()),
            Verdict(VerdictStatus::ok(), 20),
            Verdict(VerdictStatus::ok(), 30)};

    EXPECT_THAT(aggregator.aggregate(verdicts, 70), Eq(Verdict(VerdictStatus::ok(), 20)));
}

}

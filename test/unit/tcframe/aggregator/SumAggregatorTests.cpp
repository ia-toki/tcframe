#include "gmock/gmock.h"

#include "tcframe/aggregator/SumAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SumAggregatorTests : public Test {
protected:
    SumAggregator aggregator;
};

TEST_F(SumAggregatorTests, Aggregate_FullPoints) {
    vector<Verdict> verdicts = {
            Verdict(VerdictStatus::ac()),
            Verdict(VerdictStatus::ac())};
    EXPECT_THAT(aggregator.aggregate(verdicts, 100), Eq(Verdict(VerdictStatus::ac(), 100)));
}

TEST_F(SumAggregatorTests, Aggregate_PartialPoints) {
    vector<Verdict> verdicts = {
            Verdict(VerdictStatus::ac()),
            Verdict(VerdictStatus::tle()),
            Verdict(VerdictStatus::ok(), 30),
            Verdict(VerdictStatus::wa())};
    EXPECT_THAT(aggregator.aggregate(verdicts, 100), Eq(Verdict(VerdictStatus::tle(), 55)));
}

TEST_F(SumAggregatorTests, Aggregate_EmptyVerdicts) {
    EXPECT_THAT(aggregator.aggregate(vector<Verdict>(), 100), Eq(Verdict(VerdictStatus::ac(), 100)));
}

}

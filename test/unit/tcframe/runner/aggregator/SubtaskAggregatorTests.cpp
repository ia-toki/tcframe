#include "gmock/gmock.h"

#include "tcframe/runner/aggregator/SubtaskAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SubtaskAggregatorTests : public Test {
protected:
    SubtaskAggregator aggregator;
};

TEST_F(SubtaskAggregatorTests, Aggregate) {
    vector<SubtaskVerdict> verdicts = {
            SubtaskVerdict(Verdict::ac(), 10),
            SubtaskVerdict(Verdict::tle(), 20),
            SubtaskVerdict(Verdict::ok(), 30),
            SubtaskVerdict(Verdict::wa(), 0)};
    EXPECT_THAT(aggregator.aggregate(verdicts), Eq(SubtaskVerdict(Verdict::tle(), 60)));
}

}

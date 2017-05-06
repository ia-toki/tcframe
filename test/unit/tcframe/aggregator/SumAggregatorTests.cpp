#include "gmock/gmock.h"
#include "../mock.hpp"

#include "tcframe/aggregator/SumAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SumAggregatorTests : public Test {
protected:
    SumAggregator aggregator;
};

TEST_F(SumAggregatorTests, Aggregate) {
    EXPECT_THAT(aggregator.aggregate({
            Verdict(VerdictStatus::wa()), Verdict(VerdictStatus::tle()), Verdict(VerdictStatus::rte())}),
            Eq(Verdict(VerdictStatus::tle())));
}

}

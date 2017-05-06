#include "gmock/gmock.h"
#include "../mock.hpp"

#include "tcframe/aggregator/MinAggregator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class MinAggregatorTests : public Test {
protected:
    MinAggregator aggregator;
};

TEST_F(MinAggregatorTests, Aggregate) {
    EXPECT_THAT(aggregator.aggregate({
            Verdict(VerdictStatus::wa()), Verdict(VerdictStatus::tle()), Verdict(VerdictStatus::rte())}),
            Eq(Verdict(VerdictStatus::tle())));
}

}

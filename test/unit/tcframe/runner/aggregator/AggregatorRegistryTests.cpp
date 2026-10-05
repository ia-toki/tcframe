#include "gmock/gmock.h"

#include <stdexcept>

#include "tcframe/runner/aggregator/AggregatorRegistry.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class AggregatorRegistryTests : public Test {
protected:
    AggregatorRegistry registry;
};

TEST_F(AggregatorRegistryTests, Undeclared_WithSubtasks_Min) {
    auto aggregator = registry.getTestCaseAggregator("", "", true);
    EXPECT_NE(dynamic_cast<MinAggregator*>(aggregator), nullptr);
}

TEST_F(AggregatorRegistryTests, Undeclared_WithoutSubtasks_Sum) {
    auto aggregator = registry.getTestCaseAggregator("", "", false);
    EXPECT_NE(dynamic_cast<SumAggregator*>(aggregator), nullptr);
}

TEST_F(AggregatorRegistryTests, Declared_Min) {
    auto aggregator = registry.getTestCaseAggregator("min", "", false);
    EXPECT_NE(dynamic_cast<MinAggregator*>(aggregator), nullptr);
}

TEST_F(AggregatorRegistryTests, Declared_Sum) {
    auto aggregator = registry.getTestCaseAggregator("sum", "", true);
    EXPECT_NE(dynamic_cast<SumAggregator*>(aggregator), nullptr);
}

TEST_F(AggregatorRegistryTests, Declared_Threshold_UsesArgs) {
    auto aggregator = registry.getTestCaseAggregator("threshold", "18", true);
    EXPECT_THAT(aggregator->aggregate({TestCaseVerdict(Verdict::ok(), 18)}, 25),
            Eq(SubtaskVerdict(Verdict::ac(), 25)));
    EXPECT_THAT(aggregator->aggregate({TestCaseVerdict(Verdict::ok(), 19)}, 25),
            Eq(SubtaskVerdict(Verdict::wa(), 0)));
}

TEST_F(AggregatorRegistryTests, Declared_Threshold_FractionalArgs) {
    auto aggregator = registry.getTestCaseAggregator("threshold", "2.5", true);
    EXPECT_THAT(aggregator->aggregate({TestCaseVerdict(Verdict::ok(), 2.5)}, 10),
            Eq(SubtaskVerdict(Verdict::ac(), 10)));
}

TEST_F(AggregatorRegistryTests, Declared_Threshold_NonNumericArgs_Throws) {
    EXPECT_THROW(registry.getTestCaseAggregator("threshold", "", true), std::invalid_argument);
    EXPECT_THROW(registry.getTestCaseAggregator("threshold", "18x", true), std::invalid_argument);
}

TEST_F(AggregatorRegistryTests, Declared_Unknown_Throws) {
    EXPECT_THROW(registry.getTestCaseAggregator("bogus", "", true), std::runtime_error);
}

}

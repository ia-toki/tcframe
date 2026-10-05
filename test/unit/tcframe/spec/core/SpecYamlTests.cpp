#include "gmock/gmock.h"

#include "tcframe/spec/core/SpecYaml.hpp"

using ::testing::ElementsAre;
using ::testing::IsEmpty;
using ::testing::Test;

namespace tcframe {

class SpecYamlTests : public Test {};

TEST_F(SpecYamlTests, SubtaskYamlAggregatorIsUndeclaredByDefault) {
    SubtaskYaml subtask{};

    EXPECT_THAT(subtask.aggregator.slug, IsEmpty());
    EXPECT_THAT(subtask.aggregator.args, IsEmpty());
}

TEST_F(SpecYamlTests, EvaluatorYamlSolutionKeysDefaultToSingleSourceKey) {
    EvaluatorYaml evaluator{};

    EXPECT_THAT(evaluator.solution_keys, ElementsAre("source"));
}

TEST_F(SpecYamlTests, SpecYamlHelpersAreEmptyByDefault) {
    SpecYaml spec{};

    EXPECT_THAT(spec.helpers, IsEmpty());
}

TEST_F(SpecYamlTests, HelperYamlHoldsSlugAndAdditionalArgs) {
    HelperYaml scorer{"compare", "float_tolerance 9"};

    EXPECT_EQ("compare", scorer.slug);
    EXPECT_EQ("float_tolerance 9", scorer.additional_args);
}

}

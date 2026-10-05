#include "gmock/gmock.h"

#include "tcframe/spec/config/StyleConfig.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class StyleConfigBuilderTests : public Test {
protected:
    StyleConfigBuilder builder;
};

TEST_F(StyleConfigBuilderTests, Default_NoFloatTolerance) {
    StyleConfig styleConfig = builder.build();

    EXPECT_FALSE(bool(styleConfig.floatTolerance()));
}

TEST_F(StyleConfigBuilderTests, OutputOnlyEvaluator) {
    StyleConfig styleConfig = builder.OutputOnlyEvaluator().build();

    EXPECT_THAT(styleConfig.evaluationStyle(), Eq(EvaluationStyle::OUTPUT_ONLY));
}

TEST_F(StyleConfigBuilderTests, FunctionalEvaluator_SetsSolutionKeys) {
    StyleConfig styleConfig = builder.FunctionalEvaluator({"encoder", "decoder"}).build();

    EXPECT_THAT(styleConfig.evaluationStyle(), Eq(EvaluationStyle::FUNCTIONAL));
    EXPECT_THAT(styleConfig.solutionKeys(), Eq(vector<string>{"encoder", "decoder"}));
}

TEST_F(StyleConfigBuilderTests, Default_SolutionKeysIsSource) {
    StyleConfig styleConfig = builder.build();

    EXPECT_THAT(styleConfig.solutionKeys(), Eq(vector<string>{"source"}));
}

TEST_F(StyleConfigBuilderTests, AbsoluteFloatToleranceScorer) {
    StyleConfig styleConfig = builder.AbsoluteFloatToleranceScorer(9).build();

    EXPECT_THAT(styleConfig.floatTolerance().value(), Eq(FloatTolerance{9, FloatToleranceMode::ABSOLUTE}));
}

TEST_F(StyleConfigBuilderTests, RelativeFloatToleranceScorer) {
    StyleConfig styleConfig = builder.RelativeFloatToleranceScorer(6).build();

    EXPECT_THAT(styleConfig.floatTolerance().value(), Eq(FloatTolerance{6, FloatToleranceMode::RELATIVE}));
}

TEST_F(StyleConfigBuilderTests, FloatToleranceScorer_Both) {
    StyleConfig styleConfig = builder.FloatToleranceScorer(9).build();

    EXPECT_THAT(styleConfig.floatTolerance().value(), Eq(FloatTolerance{9, FloatToleranceMode::BOTH}));
}

TEST_F(StyleConfigBuilderTests, FloatToleranceScorer_LastCallWins) {
    StyleConfig styleConfig = builder
            .AbsoluteFloatToleranceScorer(9)
            .FloatToleranceScorer(4)
            .build();

    EXPECT_THAT(styleConfig.floatTolerance().value(), Eq(FloatTolerance{4, FloatToleranceMode::BOTH}));
}

}

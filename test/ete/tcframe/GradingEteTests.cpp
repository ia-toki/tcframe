#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Test;

namespace tcframe {

class GradingEteTests : public BaseEteTests {};

TEST_F(GradingEteTests, Normal) {
    string result = exec("cd test-ete/normal && ../scripts/grade.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("normal_sample_1: Accepted"),
            HasSubstr("normal_1: Wrong Answer"),
            HasSubstr("normal_2: Runtime Error"),
            HasSubstr("normal_3: Time Limit Exceeded")));
}

TEST_F(GradingEteTests, Normal_CustomScorer) {
    string result = exec("cd test-ete/normal-custom-scorer && ../scripts/grade-with-custom-scorer.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("normal-custom-scorer_sample_1: Accepted"),
            HasSubstr("normal-custom-scorer_1: Wrong Answer"),
            HasSubstr("normal-custom-scorer_2: Internal Error"),
            HasSubstr("normal-custom-scorer_3: Accepted")));
}

}

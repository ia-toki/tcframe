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
            HasSubstr("normal_sample_1: Wrong Answer"),
            HasSubstr("normal_1: Accepted"),
            HasSubstr("normal_2: Runtime Error"),
            HasSubstr("normal_3: Time Limit Exceeded")));
    EXPECT_THAT(result, HasSubstr("Time Limit Exceeded [33.33]"));
}

TEST_F(GradingEteTests, Normal_CustomScorer) {
    string result = exec("cd test-ete/normal-custom-scorer && ../scripts/grade-with-custom-scorer.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("normal-custom-scorer_sample_1: Accepted"),
            HasSubstr("normal-custom-scorer_1: Wrong Answer"),
            HasSubstr("normal-custom-scorer_2: Internal Error"),
            HasSubstr("normal-custom-scorer_3: Accepted")));
}

TEST_F(GradingEteTests, Subtasks) {
    string result = exec("cd test-ete/subtasks && ../scripts/grade.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("subtasks_sample_1: Accepted"),
            HasSubstr("subtasks_sample_2: Wrong Answer"),
            HasSubstr("subtasks_1_1: Accepted"),
            HasSubstr("subtasks_1_2: Accepted"),
            HasSubstr("subtasks_2_1: Wrong Answer")));
    EXPECT_THAT(result, AllOf(
            HasSubstr("Subtask 1: Accepted [70]"),
            HasSubstr("Subtask 2: Wrong Answer [0]"),
            HasSubstr("Wrong Answer [70]")));
}

TEST_F(GradingEteTests, Interactive) {
    string result = exec("cd test-ete/interactive && ../scripts/grade-with-communicator.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("interactive_sample_1: Time Limit Exceeded"),
            HasSubstr("interactive_1: Accepted"),
            HasSubstr("interactive_2: Accepted"),
            HasSubstr("interactive_3: Time Limit Exceeded")));
}

}

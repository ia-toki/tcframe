#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::AllOf;
using ::testing::Eq;
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

TEST_F(GradingEteTests, Normal_Brief) {
    string result = exec("cd test-ete/normal && ../scripts/grade.sh --brief");
    EXPECT_THAT(result, Eq("TLE 33.33\n"));
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

TEST_F(GradingEteTests, FloatTolerance) {
    ASSERT_THAT(execStatus("cd test-ete/float-tolerance && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));

    string result = exec("cd test-ete/float-tolerance && ../scripts/grade.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("float-tolerance_1_1: Accepted"),
            HasSubstr("float-tolerance_2_2: Accepted"),
            HasSubstr("Subtask 1: Accepted [40]"),
            HasSubstr("Subtask 2: Accepted [60]")));
}

TEST_F(GradingEteTests, FloatTolerance_OutsideTolerance) {
    ASSERT_THAT(execStatus("cd test-ete/float-tolerance && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));

    string result = exec("cd test-ete/float-tolerance && ../scripts/grade-rounded.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("float-tolerance_1_1: Wrong Answer"),
            HasSubstr("Subtask 1: Wrong Answer [0]"),
            HasSubstr("Subtask 2: Wrong Answer [0]")));
}

TEST_F(GradingEteTests, OutputOnly) {
    ASSERT_THAT(execStatus("cd test-ete/output-only && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));
    EXPECT_THAT(readFile("test-ete/output-only/build/spec.yml"), HasSubstr("  slug: output_only"));

    string result = exec("cd test-ete/output-only && ../scripts/grade-output-only.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("output-only_1: Accepted"),
            HasSubstr("output-only_2: Accepted"),
            HasSubstr("output-only_3: Accepted"),
            HasSubstr("Accepted [100]")));
}

TEST_F(GradingEteTests, OutputOnly_WrongAndMissing) {
    ASSERT_THAT(execStatus("cd test-ete/output-only && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));

    string result = exec("cd test-ete/output-only && ../scripts/grade-output-only-wrong.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("output-only_1: Wrong Answer"),
            HasSubstr("output-only_2: Wrong Answer"),
            HasSubstr("Submitted output not found"),
            HasSubstr("output-only_3: Accepted"),
            HasSubstr("Wrong Answer [33.33]")));
}
}

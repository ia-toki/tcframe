#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::Eq;
using ::testing::Test;
using ::testing::UnorderedElementsAre;

namespace tcframe {

class GenerationEteTests : public BaseEteTests {};

TEST_F(GenerationEteTests, Normal) {
    ASSERT_THAT(execStatus("cd test-ete/normal && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/normal/tc"), UnorderedElementsAre(
           "normal_sample_1.in",
           "normal_sample_1.out",
           "normal_1.in",
           "normal_1.out",
           "normal_2.in",
           "normal_2.out",
           "normal_3.in",
           "normal_3.out"
    ));
}

TEST_F(GenerationEteTests, Normal_NoOutput) {
    ASSERT_THAT(execStatus("cd test-ete/normal-no-output && ../scripts/generate-without-solution.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/normal-no-output/tc"), UnorderedElementsAre(
            "normal-no-output_sample_1.in",
            "normal-no-output_1.in",
            "normal-no-output_2.in",
            "normal-no-output_3.in"
    ));
}

TEST_F(GenerationEteTests, Normal_ComplexFormats) {
    ASSERT_THAT(execStatus("cd test-ete/normal-complex-formats && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/normal-complex-formats/tc"), UnorderedElementsAre(
            "normal-complex-formats_sample_1.in",
            "normal-complex-formats_sample_1.out",
            "normal-complex-formats_1.in",
            "normal-complex-formats_1.out"
    ));
}

TEST_F(GenerationEteTests, Normal_CustomScorer) {
    ASSERT_THAT(execStatus("cd test-ete/normal-custom-scorer && ../scripts/generate-with-custom-scorer.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/normal-custom-scorer/tc"), UnorderedElementsAre(
            "normal-custom-scorer_sample_1.in",
            "normal-custom-scorer_sample_1.out",
            "normal-custom-scorer_1.in",
            "normal-custom-scorer_1.out",
            "normal-custom-scorer_2.in",
            "normal-custom-scorer_2.out",
            "normal-custom-scorer_3.in",
            "normal-custom-scorer_3.out"
    ));
}

TEST_F(GenerationEteTests, Subtasks) {
    ASSERT_THAT(execStatus("cd test-ete/subtasks && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/subtasks/tc"), UnorderedElementsAre(
            "subtasks_sample_1.in",
            "subtasks_sample_1.out",
            "subtasks_sample_2.in",
            "subtasks_sample_2.out",
            "subtasks_1_1.in",
            "subtasks_1_1.out",
            "subtasks_1_2.in",
            "subtasks_1_2.out",
            "subtasks_2_1.in",
            "subtasks_2_1.out"
    ));
}

TEST_F(GenerationEteTests, Multi) {
    ASSERT_THAT(execStatus("cd test-ete/multi && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/multi/tc"), UnorderedElementsAre(
            "multi_sample.in",
            "multi_sample.out",
            "multi_1.in",
            "multi_1.out",
            "multi_2.in",
            "multi_2.out"
    ));

    EXPECT_THAT(readFile("test-ete/multi/tc/multi_1.in"), Eq(
            "2\n"
            "1 3\n"
            "2 4\n"));

    EXPECT_THAT(readFile("test-ete/multi/tc/multi_sample.out"), Eq(
            "6\n"
            "11\n"));

    EXPECT_THAT(readFile("test-ete/multi/tc/multi_1.out"), Eq(
            "4\n"
            "6\n"));
}

TEST_F(GenerationEteTests, Multi_NoOutput) {
    ASSERT_THAT(execStatus("cd test-ete/multi-no-output && ../scripts/generate-without-solution.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/multi-no-output/tc"), UnorderedElementsAre(
            "multi-no-output_sample.in",
            "multi-no-output_1.in",
            "multi-no-output_2.in"
    ));
}

TEST_F(GenerationEteTests, Multi_WithOutputPrefix) {
    ASSERT_THAT(execStatus("cd test-ete/multi-prefix && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("test-ete/multi-prefix/tc"), UnorderedElementsAre(
            "multi-prefix_sample.in",
            "multi-prefix_sample.out",
            "multi-prefix_1.in",
            "multi-prefix_1.out",
            "multi-prefix_2.in",
            "multi-prefix_2.out"
    ));

    EXPECT_THAT(readFile("test-ete/multi-prefix/tc/multi-prefix_sample.out"), Eq(
            "Case\t\"$1\\\":\n"
            "6\n"
            "Case\t\"$2\\\":\n"
            "11\n"));

    EXPECT_THAT(readFile("test-ete/multi-prefix/tc/multi-prefix_1.out"), Eq(
            "Case\t\"$1\\\":\n"
            "4\n"
            "Case\t\"$2\\\":\n"
            "6\n"));
}

}

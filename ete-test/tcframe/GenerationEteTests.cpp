#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::Eq;
using ::testing::Test;
using ::testing::UnorderedElementsAre;

namespace tcframe {

class GenerationEteTests : public BaseEteTests {};

TEST_F(GenerationEteTests, Normal) {
    ASSERT_THAT(execStatus("cd ete/normal && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("ete/normal/tc"), UnorderedElementsAre(
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

TEST_F(GenerationEteTests, NotGeneratingOutput) {
    ASSERT_THAT(execStatus("cd ete/no-output && ../scripts/generate_without_solution.sh"), Eq(0));

    EXPECT_THAT(ls("ete/no-output/tc"), UnorderedElementsAre(
            "no-output_sample_1.in",
            "no-output_1.in",
            "no-output_2.in",
            "no-output_3.in"
    ));
}

TEST_F(GenerationEteTests, Normal_ComplexFormats) {
    ASSERT_THAT(execStatus("cd ete/normal-complex-formats && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("ete/normal-complex-formats/tc"), UnorderedElementsAre(
            "normal-complex-formats_sample_1.in",
            "normal-complex-formats_sample_1.out",
            "normal-complex-formats_1.in",
            "normal-complex-formats_1.out"
    ));
}

TEST_F(GenerationEteTests, Subtasks) {
    ASSERT_THAT(execStatus("cd ete/subtasks && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("ete/subtasks/tc"), UnorderedElementsAre(
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
    ASSERT_THAT(execStatus("cd ete/multi && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("ete/multi/tc"), UnorderedElementsAre(
            "multi_sample.in",
            "multi_sample.out",
            "multi_1.in",
            "multi_1.out",
            "multi_2.in",
            "multi_2.out"
    ));

    EXPECT_THAT(readFile("ete/multi/tc/multi_sample.out"), Eq(
            "6\n"
            "11\n"));

    EXPECT_THAT(readFile("ete/multi/tc/multi_1.out"), Eq(
            "4\n"
            "6\n"));
}

TEST_F(GenerationEteTests, Multi_WithOutputPrefix) {
    ASSERT_THAT(execStatus("cd ete/multi-prefix && ../scripts/generate.sh"), Eq(0));

    EXPECT_THAT(ls("ete/multi-prefix/tc"), UnorderedElementsAre(
            "multi-prefix_sample.in",
            "multi-prefix_sample.out",
            "multi-prefix_1.in",
            "multi-prefix_1.out",
            "multi-prefix_2.in",
            "multi-prefix_2.out"
    ));

    EXPECT_THAT(readFile("ete/multi-prefix/tc/multi-prefix_sample.out"), Eq(
            "Case\t\"$1\\\":\n"
            "6\n"
            "Case\t\"$2\\\":\n"
            "11\n"));

    EXPECT_THAT(readFile("ete/multi-prefix/tc/multi-prefix_1.out"), Eq(
            "Case\t\"$1\\\":\n"
            "4\n"
            "Case\t\"$2\\\":\n"
            "6\n"));
}

}

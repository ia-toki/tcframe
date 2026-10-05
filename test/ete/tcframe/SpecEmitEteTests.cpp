#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class SpecEmitEteTests : public BaseEteTests {};

TEST_F(SpecEmitEteTests, Normal_WritesSpecYaml) {
    ASSERT_THAT(execStatus("cd test-ete/spec-emit && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));
    ASSERT_THAT(execStatus("cd test-ete/spec-emit && rm -f spec.yml && ./build/spec spec"), Eq(0));

    EXPECT_THAT(readFile("test-ete/spec-emit/spec.yml"), StrEq(
            "slug: spec-emit\n"
            "subtasks:\n"
            "  - points: 70\n"
            "  - points: 30\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [source]\n"
            "  tc_output_present: true\n"
            "helpers: {}\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecEmitEteTests, Normal_AppliesPythonDiscoveredInputs) {
    ASSERT_THAT(execStatus("cd test-ete/spec-emit && ./build/spec spec --spec-file=with-overrides.yml"
            " --solution-keys=encoder,decoder"
            " --helper='scorer=compare' --helper-args='scorer=float_tolerance 9'"
            " --aggregator=1=min --aggregator=2=threshold --aggregator-args=2=16"), Eq(0));

    EXPECT_THAT(readFile("test-ete/spec-emit/with-overrides.yml"), StrEq(
            "slug: spec-emit\n"
            "subtasks:\n"
            "  - points: 70\n"
            "    aggregator:\n"
            "      slug: min\n"
            "  - points: 30\n"
            "    aggregator:\n"
            "      slug: threshold\n"
            "      args: 16\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [encoder, decoder]\n"
            "  tc_output_present: true\n"
            "helpers:\n"
            "  scorer:\n"
            "    slug: compare\n"
            "    additional_args: \"float_tolerance 9\"\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecEmitEteTests, Normal_SpecFileFlag) {
    ASSERT_THAT(execStatus("cd test-ete/spec-emit && ./build/spec spec --spec-file=custom-spec.yml"), Eq(0));

    EXPECT_THAT(ls("test-ete/spec-emit"), ::testing::Contains("custom-spec.yml"));
}

TEST_F(SpecEmitEteTests, Normal_StyleConfigAndSubtaskAggregators) {
    ASSERT_THAT(execStatus("cd test-ete/float-tolerance && g++ -o solution solution.cpp && TCFRAME_HOME=$(cd ../../tcframe && pwd) ../../tcframe/scripts/tcframe build --solution=./solution"), Eq(0));
    ASSERT_THAT(execStatus("cd test-ete/float-tolerance && rm -f spec.yml && ./build/spec spec"), Eq(0));

    EXPECT_THAT(readFile("test-ete/float-tolerance/spec.yml"), StrEq(
            "slug: float-tolerance\n"
            "subtasks:\n"
            "  - points: 40\n"
            "    aggregator:\n"
            "      slug: threshold\n"
            "      args: 18\n"
            "  - points: 60\n"
            "    aggregator:\n"
            "      slug: min\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [source]\n"
            "  tc_output_present: true\n"
            "helpers:\n"
            "  scorer:\n"
            "    additional_args: \"float_absolute_tolerance 1e-06\"\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}
}

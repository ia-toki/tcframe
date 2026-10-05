#include "gmock/gmock.h"

#include "tcframe/spec/core/SpecYamlEmitter.hpp"

using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class SpecYamlEmitterTests : public Test {
protected:
    SpecYaml makeMinimalSpec() {
        SpecYaml spec;
        spec.slug = "aplusb";
        spec.evaluator.slug = "batch";
        spec.evaluator.tc_output_present = true;
        spec.limits.time_s = 2;
        spec.limits.memory_mb = 64;
        return spec;
    }
};

TEST_F(SpecYamlEmitterTests, EmitsMinimalSpecWithEmptySubtasksAndHelpers) {
    SpecYaml spec = makeMinimalSpec();

    EXPECT_THAT(SpecYamlEmitter::emit(spec), StrEq(
            "slug: aplusb\n"
            "subtasks: []\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [source]\n"
            "  tc_output_present: true\n"
            "helpers: {}\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsSubtasksWithAndWithoutAggregator) {
    SpecYaml spec = makeMinimalSpec();
    spec.subtasks = {
            SubtaskYaml{25, AggregatorYaml{"min", ""}},
            SubtaskYaml{25, AggregatorYaml{"threshold", "16"}},
            SubtaskYaml{50, AggregatorYaml{"", ""}},
    };

    EXPECT_THAT(SpecYamlEmitter::emit(spec), StrEq(
            "slug: aplusb\n"
            "subtasks:\n"
            "  - points: 25\n"
            "    aggregator:\n"
            "      slug: min\n"
            "  - points: 25\n"
            "    aggregator:\n"
            "      slug: threshold\n"
            "      args: 16\n"
            "  - points: 50\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [source]\n"
            "  tc_output_present: true\n"
            "helpers: {}\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsAggregatorArgsEvenWithoutSlug) {
    SpecYaml spec = makeMinimalSpec();
    spec.subtasks = {SubtaskYaml{50, AggregatorYaml{"", "16"}}};

    EXPECT_THAT(SpecYamlEmitter::emit(spec), ::testing::HasSubstr(
            "  - points: 50\n"
            "    aggregator:\n"
            "      args: 16\n"));
}

TEST_F(SpecYamlEmitterTests, OmitsEvaluatorSlugForCustomEvaluator) {
    SpecYaml spec = makeMinimalSpec();
    spec.evaluator.slug = "";

    EXPECT_THAT(SpecYamlEmitter::emit(spec), ::testing::HasSubstr(
            "evaluator:\n"
            "  solution_keys: [source]\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsMultipleSolutionKeys) {
    SpecYaml spec = makeMinimalSpec();
    spec.evaluator.slug = "functional";
    spec.evaluator.solution_keys = {"encoder", "decoder"};

    EXPECT_THAT(SpecYamlEmitter::emit(spec), ::testing::HasSubstr(
            "  solution_keys: [encoder, decoder]\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsHelpersWithSlugAndArgsOrEmptyFlowMap) {
    SpecYaml spec = makeMinimalSpec();
    spec.helpers["scorer"] = HelperYaml{"compare", "float_tolerance 9"};
    spec.helpers["communicator"] = HelperYaml{"", ""};

    EXPECT_THAT(SpecYamlEmitter::emit(spec), StrEq(
            "slug: aplusb\n"
            "subtasks: []\n"
            "evaluator:\n"
            "  slug: batch\n"
            "  solution_keys: [source]\n"
            "  tc_output_present: true\n"
            "helpers:\n"
            "  communicator: {}\n"
            "  scorer:\n"
            "    slug: compare\n"
            "    additional_args: \"float_tolerance 9\"\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsFractionalPointsWithoutTrailingZeros) {
    SpecYaml spec = makeMinimalSpec();
    spec.subtasks = {SubtaskYaml{12.5, AggregatorYaml{"", ""}}};

    EXPECT_THAT(SpecYamlEmitter::emit(spec), ::testing::HasSubstr("  - points: 12.5\n"));
}

TEST_F(SpecYamlEmitterTests, QuotesReservedWordsAndUnsafeScalars) {
    SpecYaml spec = makeMinimalSpec();
    spec.slug = "yes";
    spec.evaluator.slug = "a: b";
    spec.evaluator.solution_keys = {"say \"hi\""};

    EXPECT_THAT(SpecYamlEmitter::emit(spec), StrEq(
            "slug: \"yes\"\n"
            "subtasks: []\n"
            "evaluator:\n"
            "  slug: \"a: b\"\n"
            "  solution_keys: [\"say \\\"hi\\\"\"]\n"
            "  tc_output_present: true\n"
            "helpers: {}\n"
            "limits:\n"
            "  time: 2000\n"
            "  memory: 65536\n"));
}

TEST_F(SpecYamlEmitterTests, EmitsFalseForTcOutputNotPresent) {
    SpecYaml spec = makeMinimalSpec();
    spec.evaluator.tc_output_present = false;

    EXPECT_THAT(SpecYamlEmitter::emit(spec), ::testing::HasSubstr("  tc_output_present: false\n"));
}

}

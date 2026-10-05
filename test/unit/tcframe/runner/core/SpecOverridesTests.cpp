#include "gmock/gmock.h"

#include "tcframe/runner/core/ArgsParser.hpp"
#include "tcframe/runner/core/SpecOverrides.hpp"

using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::IsEmpty;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class SpecOverridesTests : public Test {
protected:
    SpecYaml makeSpecWithTwoSubtasks() {
        SpecYaml spec;
        spec.slug = "aplusb";
        spec.subtasks = {SubtaskYaml{70, AggregatorYaml{}}, SubtaskYaml{30, AggregatorYaml{}}};
        return spec;
    }

    Args parseSpecArgs(const vector<string>& opts = {}) {
        vector<string> storage = {"./runner", "spec"};
        storage.insert(storage.end(), opts.begin(), opts.end());
        vector<char*> argv;
        for (string& s : storage) {
            argv.push_back(&s[0]);
        }
        argv.push_back(nullptr);
        return ArgsParser::parse(int(argv.size()) - 1, argv.data());
    }
};

TEST_F(SpecOverridesTests, NoOverridesLeavesSpecUntouched) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    SpecOverrides::apply(parseSpecArgs(), spec);

    EXPECT_THAT(spec.evaluator.solution_keys, ElementsAre("source"));
    EXPECT_THAT(spec.helpers, IsEmpty());
    EXPECT_THAT(spec.subtasks[0].aggregator.slug, IsEmpty());
}

TEST_F(SpecOverridesTests, AppliesSolutionKeys) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    SpecOverrides::apply(parseSpecArgs({"--solution-keys=encoder,decoder"}), spec);

    EXPECT_THAT(spec.evaluator.solution_keys, ElementsAre("encoder", "decoder"));
}

TEST_F(SpecOverridesTests, AppliesHelperSlugAndArgs) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    SpecOverrides::apply(parseSpecArgs({"--helper=scorer=compare",
            "--helper-args=scorer=float_tolerance 9",
            "--helper-args=communicator=python Comm.py"}), spec);

    EXPECT_THAT(spec.helpers["scorer"].slug, StrEq("compare"));
    EXPECT_THAT(spec.helpers["scorer"].additional_args, StrEq("float_tolerance 9"));
    EXPECT_THAT(spec.helpers["communicator"].slug, IsEmpty());
    EXPECT_THAT(spec.helpers["communicator"].additional_args, StrEq("python Comm.py"));
}

TEST_F(SpecOverridesTests, AppliesAggregatorSlugAndArgsByOneBasedIndex) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    SpecOverrides::apply(parseSpecArgs({"--aggregator=1=min",
            "--aggregator=2=threshold",
            "--aggregator-args=2=16"}), spec);

    EXPECT_THAT(spec.subtasks[0].aggregator.slug, StrEq("min"));
    EXPECT_THAT(spec.subtasks[0].aggregator.args, IsEmpty());
    EXPECT_THAT(spec.subtasks[1].aggregator.slug, StrEq("threshold"));
    EXPECT_THAT(spec.subtasks[1].aggregator.args, StrEq("16"));
}

TEST_F(SpecOverridesTests, RejectsAggregatorIndexOutOfRange) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    try {
        SpecOverrides::apply(parseSpecArgs({"--aggregator=3=min"}), spec);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: --aggregator subtask index '3' is out of range"));
    }
}

TEST_F(SpecOverridesTests, RejectsMalformedHelper) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    try {
        SpecOverrides::apply(parseSpecArgs({"--helper=scorer"}), spec);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: --helper expects key=value, got 'scorer'"));
    }
}

TEST_F(SpecOverridesTests, RejectsEmptySolutionKeys) {
    SpecYaml spec = makeSpecWithTwoSubtasks();

    try {
        SpecOverrides::apply(parseSpecArgs({"--solution-keys="}), spec);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: --solution-keys must not be empty"));
    }
}

}

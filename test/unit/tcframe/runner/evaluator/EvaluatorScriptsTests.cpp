#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/evaluator/EvaluatorScripts.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class EvaluatorScriptsTests : public Test {
protected:
    MOCK(OperatingSystem) os;

    // openForReading always returns a stream; only a good one means the script file exists.
    istringstream existing = istringstream("");
    istringstream missing = istringstream("");

    void SetUp() {
        missing.setstate(ios::failbit);
    }
};

TEST_F(EvaluatorScriptsTests, HasBuild_ReturnsTrueWhenScriptExists) {
    EvaluatorScripts scripts(&os, "registry/evaluators/functional");

    EXPECT_CALL(os, openForReading("registry/evaluators/functional/build_pascal"))
            .WillOnce(Return(&existing));
    EXPECT_CALL(os, closeOpenedStream(_));

    EXPECT_TRUE(scripts.hasBuild("pascal"));
}

TEST_F(EvaluatorScriptsTests, HasRun_ReturnsFalseWhenScriptMissing) {
    EvaluatorScripts scripts(&os, "registry/evaluators/functional");

    EXPECT_CALL(os, openForReading("registry/evaluators/functional/run_pascal"))
            .WillOnce(Return(&missing));
    EXPECT_CALL(os, closeOpenedStream(_));

    EXPECT_FALSE(scripts.hasRun("pascal"));
}

TEST_F(EvaluatorScriptsTests, HasBuild_NoDirectoryMeansNoScripts) {
    EvaluatorScripts scripts(&os, "");

    EXPECT_CALL(os, openForReading(_)).Times(0);

    EXPECT_FALSE(scripts.hasBuild("cpp"));
    EXPECT_FALSE(scripts.hasRun("cpp"));
}

TEST_F(EvaluatorScriptsTests, BuildCommand_SolutionFilesThenHelperArgs) {
    EvaluatorScripts scripts(&os, "registry/evaluators/functional");

    EXPECT_THAT(scripts.buildCommand("pascal", {"dec.pas", "enc.pas"}, {"./manager"}),
            Eq("registry/evaluators/functional/build_pascal dec.pas enc.pas ./manager"));
}

TEST_F(EvaluatorScriptsTests, RunCommand_InputThenHelperArgs) {
    EvaluatorScripts scripts(&os, "registry/evaluators/functional");

    EXPECT_THAT(scripts.runCommand("pascal", "dir/foo_1.in", {"./manager"}),
            Eq("registry/evaluators/functional/run_pascal dir/foo_1.in ./manager"));
}

}

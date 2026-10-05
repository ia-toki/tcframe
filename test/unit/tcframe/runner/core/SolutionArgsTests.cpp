#include "gmock/gmock.h"

#include "tcframe/runner/core/ArgsParser.hpp"
#include "tcframe/runner/core/SolutionArgs.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SolutionArgsTests : public Test {
protected:
    const string defaultCommand = "./solution";

    SolutionMap build(const vector<string>& options, const vector<string>& requiredKeys) {
        vector<string> tokens = {"./runner", "grade"};
        tokens.insert(tokens.end(), options.begin(), options.end());

        vector<char*> argv;
        for (string& token : tokens) {
            argv.push_back(&token[0]);
        }
        argv.push_back(nullptr);

        Args args = ArgsParser::parse(argv.size() - 1, argv.data());
        return SolutionArgs::build(args, defaultCommand, requiredKeys);
    }
};

TEST_F(SolutionArgsTests, NoFlags_UsesDefaultSourceCommand) {
    EXPECT_THAT(build({}, {"source"}), Eq(SolutionMap("./solution")));
}

TEST_F(SolutionArgsTests, SolutionFlag_SetsSourceKey) {
    EXPECT_THAT(build({"--solution=python Sol.py"}, {"source"}), Eq(SolutionMap("python Sol.py")));
}

TEST_F(SolutionArgsTests, SolutionFiles_BuildMultipleKeys) {
    SolutionMap solutions = build({"--solution-file=encoder=enc.cpp", "--solution-file=decoder=dec.cpp"}, {"decoder", "encoder"});

    EXPECT_THAT(solutions.get("encoder"), Eq("enc.cpp"));
    EXPECT_THAT(solutions.get("decoder"), Eq("dec.cpp"));
    EXPECT_FALSE(solutions.has(SolutionMap::DEFAULT_KEY));
}

TEST_F(SolutionArgsTests, SolutionFiles_RequiredKeysOrderDoesNotMatter) {
    EXPECT_NO_THROW(build({"--solution-file=b=b.cpp", "--solution-file=a=a.cpp"}, {"b", "a"}));
}

TEST_F(SolutionArgsTests, SolutionFiles_WithSolutionFlagForSameKeyIsError) {
    EXPECT_THROW(build({"--solution=python Sol.py", "--solution-file=source=sol.cpp"}, {"source"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_MissingRequiredKeyIsError) {
    EXPECT_THROW(build({"--solution-file=encoder=enc.cpp"}, {"decoder", "encoder"}), runtime_error);
}

TEST_F(SolutionArgsTests, NoFlags_WhenKeysRequiredOtherThanSourceIsError) {
    EXPECT_THROW(build({}, {"encoder", "decoder"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_UnexpectedKeyIsError) {
    EXPECT_THROW(build({"--solution-file=encoder=enc.cpp"}, {"source"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_DuplicateKeyIsError) {
    EXPECT_THROW(build({"--solution-file=encoder=a.cpp", "--solution-file=encoder=b.cpp"}, {"encoder"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_MissingSeparatorIsError) {
    EXPECT_THROW(build({"--solution-file=encoder"}, {"encoder"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_EmptyKeyIsError) {
    EXPECT_THROW(build({"--solution-file==enc.cpp"}, {"source"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_InvalidKeyCharacterIsError) {
    EXPECT_THROW(build({"--solution-file=en-coder=enc.cpp"}, {"en-coder"}), runtime_error);
}

TEST_F(SolutionArgsTests, SolutionFiles_EmptyPathIsError) {
    EXPECT_THROW(build({"--solution-file=encoder="}, {"encoder"}), runtime_error);
}

}

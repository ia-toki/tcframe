#include "gmock/gmock.h"

#include "tcframe/runner/ArgsParser.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class ArgsParserTests : public Test {};

TEST_F(ArgsParserTests, Parsing_AllOptions) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--communicator=python Communicator.py",
            (char*) "--memory-limit=128",
            (char*) "--output=my/testdata",
            (char*) "--scorer=python Scorer.py",
            (char*) "--seed=42",
            (char*) "--solution=python Sol.py",
            (char*) "--time-limit=3",
            nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    Args args = ArgsParser::parse(argc, argv);
    EXPECT_FALSE(args.noMemoryLimit());
    EXPECT_FALSE(args.noTimeLimit());

    EXPECT_THAT(args.communicator(), Eq(optional<string>("python Communicator.py")));
    EXPECT_THAT(args.memoryLimit(), Eq(optional<int>(128)));
    EXPECT_THAT(args.output(), Eq(optional<string>("my/testdata")));
    EXPECT_THAT(args.scorer(), Eq(optional<string>("python Scorer.py")));
    EXPECT_THAT(args.seed(), Eq(optional<unsigned>(42)));
    EXPECT_THAT(args.solution(), Eq(optional<string>("python Sol.py")));
    EXPECT_THAT(args.timeLimit(), Eq(optional<int>(3)));
}

TEST_F(ArgsParserTests, Parsing_SomeOptions) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--no-memory-limit",
            (char*) "--no-time-limit",
            (char*) "--output=my/testdata",
            (char*) "--seed=42",
            nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    Args args = ArgsParser::parse(argc, argv);
    EXPECT_TRUE(args.noMemoryLimit());
    EXPECT_TRUE(args.noTimeLimit());
    EXPECT_THAT(args.output(), Eq(optional<string>("my/testdata")));
    EXPECT_THAT(args.seed(), Eq(optional<unsigned>(42)));
    EXPECT_THAT(args.solution(), Eq(optional<string>()));
}

TEST_F(ArgsParserTests, Parsing_MissingOptionArgument) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--seed",
            nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    try {
        ArgsParser::parse(argc, argv);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: option --seed requires an argument"));
    }
}

TEST_F(ArgsParserTests, Parsing_InvalidOption) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--blah",
            nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    try {
        ArgsParser::parse(argc, argv);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: option --blah unknown"));
    }
}

}

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
            (char*) "--slug=foo",
            (char*) "--solution=python Sol.py",
            (char*) "--tc-dir=my/testdata",
            (char*) "--seed=42"};
    int argc = sizeof(argv) / sizeof(char*);

    Args args = ArgsParser::parse(argc, argv);
    EXPECT_THAT(args.slug(), Eq(optional<string>("foo")));
    EXPECT_THAT(args.solution(), Eq(optional<string>("python Sol.py")));
    EXPECT_THAT(args.tcDir(), Eq(optional<string>("my/testdata")));
    EXPECT_THAT(args.seed(), Eq(optional<unsigned>(42)));
}

TEST_F(ArgsParserTests, Parsing_SomeOptions) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--slug=foo",
            (char*) "--tc-dir=my/testdata",
            (char*) "--seed=42"};
    int argc = sizeof(argv) / sizeof(char*);

    Args args = ArgsParser::parse(argc, argv);
    EXPECT_THAT(args.slug(), Eq(optional<string>("foo")));
    EXPECT_THAT(args.solution(), Eq(optional<string>()));
    EXPECT_THAT(args.tcDir(), Eq(optional<string>("my/testdata")));
    EXPECT_THAT(args.seed(), Eq(optional<unsigned>(42)));
}

TEST_F(ArgsParserTests, Parsing_MissingOptionArgument) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--slug"};
    int argc = sizeof(argv) / sizeof(char*);

    try {
        ArgsParser::parse(argc, argv);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: option --slug requires an argument"));
    }
}

TEST_F(ArgsParserTests, Parsing_InvalidOption) {
    char* argv[] = {
            (char*) "./runner",
            (char*) "--blah"};
    int argc = sizeof(argv) / sizeof(char*);

    try {
        ArgsParser::parse(argc, argv);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("tcframe: option --blah unknown"));
    }
}

}

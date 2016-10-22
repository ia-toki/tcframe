#include "gmock/gmock.h"

#include "tcframe/runner/SimpleYamlParser.hpp"

using ::testing::Pair;
using ::testing::Test;
using ::testing::UnorderedElementsAre;

namespace tcframe {

class SimpleYamlParserTests : public Test {};

TEST_F(SimpleYamlParserTests, Parse) {
    string content =
            "foo:abc123\n"
            "# bar: asdf \n"
            " baz   : two words # with comments\n"
            "\n"
            "qux:  \n"
            "trashtrash";

    EXPECT_THAT(SimpleYamlParser::parse(content), UnorderedElementsAre(
            Pair("foo", "abc123"),
            Pair("baz", "two words"),
            Pair("qux", "")));
}

}

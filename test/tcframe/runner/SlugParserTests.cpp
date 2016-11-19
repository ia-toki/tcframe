#include "gmock/gmock.h"

#include "tcframe/runner/SlugParser.hpp"

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class SlugParserTests : public Test {};

TEST_F(SlugParserTests, Valid) {
    EXPECT_THAT(SlugParser::parse("/Users/fushar/contest/tree/spec.cpp"), Eq("tree"));
    EXPECT_THAT(SlugParser::parse("/Users/fushar/january contest/tree/spec.cpp"), Eq("tree"));
}

TEST_F(SlugParserTests, Valid_WithUnderscorePrefix) {
    EXPECT_THAT(SlugParser::parse("/Users/fushar/contest/a_tree/spec.cpp"), Eq("tree"));
    EXPECT_THAT(SlugParser::parse("/Users/fushar/contest/Qual2016_A_tree/spec.cpp"), Eq("tree"));
}

TEST_F(SlugParserTests, Invalid) {
    try {
        SlugParser::parse("/Users/fushar/contest/a plus b/spec.cpp");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Slug must match [a-zA-Z0-9\\-]+, but found: 'a plus b'"));
    }
}

TEST_F(SlugParserTests, Invalid_Root) {
    try {
        SlugParser::parse("/spec.cpp");
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Slug must match [a-zA-Z0-9\\-]+, but found: ''"));
    }
}


}

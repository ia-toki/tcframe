#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/SolutionMap.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class SolutionMapTests : public Test {};

TEST_F(SolutionMapTests, CommandConstructor_UsesDefaultKey) {
    SolutionMap solutions("python Sol.py");

    EXPECT_TRUE(solutions.has(SolutionMap::DEFAULT_KEY));
    EXPECT_THAT(solutions.get(SolutionMap::DEFAULT_KEY), Eq("python Sol.py"));
    EXPECT_THAT(solutions.keys(), Eq(vector<string>{"source"}));
}

TEST_F(SolutionMapTests, Set_AddsMultipleKeysSorted) {
    SolutionMap solutions;
    solutions.set("encoder", "enc.cpp");
    solutions.set("decoder", "dec.cpp");

    EXPECT_THAT(solutions.keys(), Eq(vector<string>{"decoder", "encoder"}));
    EXPECT_THAT(solutions.get("decoder"), Eq("dec.cpp"));
    EXPECT_THAT(solutions.get("encoder"), Eq("enc.cpp"));
    EXPECT_FALSE(solutions.has(SolutionMap::DEFAULT_KEY));
}

TEST_F(SolutionMapTests, Set_OverwritesExistingKey) {
    SolutionMap solutions("old");
    solutions.set(SolutionMap::DEFAULT_KEY, "new");

    EXPECT_THAT(solutions.get(SolutionMap::DEFAULT_KEY), Eq("new"));
    EXPECT_THAT(solutions.keys().size(), Eq(1u));
}

TEST_F(SolutionMapTests, Get_MissingKeyThrows) {
    SolutionMap solutions;

    EXPECT_THROW(solutions.get("source"), runtime_error);
}

TEST_F(SolutionMapTests, Equality) {
    SolutionMap a("sol");
    SolutionMap b;
    b.set(SolutionMap::DEFAULT_KEY, "sol");

    EXPECT_TRUE(a == b);

    b.set("extra", "x");
    EXPECT_FALSE(a == b);
}

}

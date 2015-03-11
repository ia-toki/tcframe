#include "gtest/gtest.h"

#include "tcframe/failure.hpp"

#include <vector>

using std::vector;
using tcframe::Failure;
using tcframe::FailuresCollector;

TEST(FailureTest, Construction) {
    Failure failure("some text", 7);

    EXPECT_EQ("some text", failure.getMessage());
    EXPECT_EQ(7, failure.getLevel());
}

TEST(FailuresCollectorTest, Collection) {
    FailuresCollector collector;

    collector.addFailure(Failure("foo", 0));
    collector.addFailures({Failure("bar", 1), Failure("baz", 2)});

    vector<Failure> failures = collector.collectFailures();

    ASSERT_EQ(3, failures.size());
    EXPECT_EQ(Failure("foo", 0), failures[0]);
    EXPECT_EQ(Failure("bar", 1), failures[1]);
    EXPECT_EQ(Failure("baz", 2), failures[2]);
}
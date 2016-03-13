#include "gtest/gtest.h"

#include "tcframe/core/Failure.hpp"
#include "tcframe/os/ExecutionException.hpp"

using tcframe::ExecutionException;
using tcframe::Failure;

TEST(ExecutionException, Construction) {
    ExecutionException e({Failure("foo", 0)});

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("", e.getMessage());
}

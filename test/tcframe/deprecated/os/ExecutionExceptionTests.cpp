#include "gtest/gtest.h"

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/os/ExecutionException.hpp"

using tcframe_old::ExecutionException;
using tcframe_old::Failure;

TEST(DeprecatedExecutionException, Construction) {
    ExecutionException e({Failure("foo", 0)});

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("", e.getMessage());
}

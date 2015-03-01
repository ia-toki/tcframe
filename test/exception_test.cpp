#include "gtest/gtest.h"

#include "tcframe/exception.hpp"

using tcframe::TestCaseException;

TEST(TestCaseExceptionTest, Construction) {
    TestCaseException e({nullptr, nullptr});

    EXPECT_EQ(2, e.getFailures().size());
}

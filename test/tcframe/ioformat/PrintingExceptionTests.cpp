#include "gtest/gtest.h"

#include "tcframe/core/Failure.hpp"
#include "tcframe/iovariable/PrintingException.hpp"

#include <vector>

using std::vector;

using tcframe::Failure;
using tcframe::PrintingException;

TEST(PrintingException, Construction) {
    PrintingException e("foo");

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("foo", e.getMessage());
}

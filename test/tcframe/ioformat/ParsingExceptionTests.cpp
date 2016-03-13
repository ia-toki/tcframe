#include "gtest/gtest.h"

#include "tcframe/core/Failure.hpp"
#include "tcframe/iovariable/ParsingException.hpp"

#include <vector>

using std::vector;

using tcframe::Failure;
using tcframe::ParsingException;

TEST(ParsingException, Construction) {
    ParsingException e("foo");

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("foo", e.getMessage());
}

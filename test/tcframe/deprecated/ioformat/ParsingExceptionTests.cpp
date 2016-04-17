#include "gtest/gtest.h"

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/iovariable/ParsingException.hpp"

#include <vector>

using std::vector;

using tcframe_old::Failure;
using tcframe_old::ParsingException;

TEST(DeprecatedParsingException, Construction) {
    ParsingException e("foo");

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("foo", e.getMessage());
}

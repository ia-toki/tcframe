#include "gtest/gtest.h"

#include "tcframe/core/Failure.hpp"
#include "tcframe/ioformat/IOFormatException.hpp"

#include <vector>

using std::vector;

using tcframe::Failure;
using tcframe::IOFormatException;

TEST(IOFormatException, Construction) {
    IOFormatException e("foo");

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("foo", e.getMessage());
}

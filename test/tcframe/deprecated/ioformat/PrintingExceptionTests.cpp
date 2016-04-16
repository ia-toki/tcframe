#include "gtest/gtest.h"

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/iovariable/PrintingException.hpp"

#include <vector>

using std::vector;

using tcframe::Failure;
using tcframe::PrintingException;

TEST(DeprecatedPrintingException, Construction) {
    PrintingException e("foo");

    EXPECT_EQ((vector<Failure>{Failure("foo", 0)}), e.getFailures());
    EXPECT_EQ("foo", e.getMessage());
}

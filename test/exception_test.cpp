#include "gtest/gtest.h"

#include "tcframe/exception.hpp"

#include <vector>

using std::vector;
using tcframe::IOFormatException;
using tcframe::IOFormatFailure;
using tcframe::SubtaskException;
using tcframe::SubtaskFailure;
using tcframe::SubtaskUnsatisfiability;

TEST(SubtaskExceptionTest, Construction) {
    SubtaskFailure* failure = new SubtaskFailure(vector<SubtaskUnsatisfiability*>{nullptr, nullptr});
    SubtaskException e(failure);

    EXPECT_EQ(failure, e.getFailure());
}

TEST(IOFormatExceptionTest, Construction) {
    IOFormatException e("some text");
    EXPECT_EQ("some text", e.getMessage());

    IOFormatFailure* failure = new IOFormatFailure("some text");
    IOFormatException e2(failure);
    EXPECT_EQ(failure, e2.getFailure());
}
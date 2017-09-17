#include "gmock/gmock.h"

#include "tcframe/exception/FormattedError.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class FormattedErrorTests : public Test {};

TEST_F(FormattedErrorTests, Combine) {
    FormattedError error1({{0, "error1a"}});
    FormattedError error2({{0, "error2a"}, {1, "error2b"}});
    FormattedError combinedError = FormattedError::combine({error1, error2});
    EXPECT_THAT(combinedError.messages(), Eq(vector<pair<int, string>>{
            {0, "error1a"},
            {0, "error2a"},
            {1, "error2b"}}));
}

}

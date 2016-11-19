#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Test;

namespace tcframe {

class GradingEteTests : public BaseEteTests {};

TEST_F(GradingEteTests, Normal) {
    string result = exec("cd ete/normal && ../scripts/grade.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("normal_sample_1: Accepted"),
            HasSubstr("normal_1: Wrong Answer"),
            HasSubstr("normal_2: Runtime Error"),
            HasSubstr("normal_3: Time Limit Exceeded")));
}

}

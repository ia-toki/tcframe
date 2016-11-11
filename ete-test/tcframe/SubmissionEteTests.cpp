#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::Test;

namespace tcframe {

class SubmissionEteTests : public BaseEteTests {};

TEST_F(SubmissionEteTests, Normal) {
    string result = exec("cd ete/normal && ../scripts/submit.sh");
    EXPECT_THAT(result, AllOf(
            HasSubstr("normal_sample_1: Accepted"),
            HasSubstr("normal_1: Wrong Answer"),
            HasSubstr("normal_2: Runtime Error"),
            HasSubstr("normal_3: Time Limit Exceeded")));
}

}

#include "gmock/gmock.h"

#include "tcframe/variable/VariableNameCreator.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class VariableNameCreatorTests : public Test {};

TEST_F(VariableNameCreatorTests, Name) {
    EXPECT_THAT(VariableNameCreator::createName("X"), Eq("'X'"));
}

TEST_F(VariableNameCreatorTests, VectorElementName) {
    EXPECT_THAT(VariableNameCreator::createVectorElementName("X", 2), Eq("'X[2]'"));
}

}

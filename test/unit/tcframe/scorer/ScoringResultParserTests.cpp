#include "gmock/gmock.h"

#include <sstream>

#include "tcframe/scorer/ScoringResultParser.hpp"

using ::testing::Eq;
using ::testing::Test;

using std::istringstream;

namespace tcframe {

class ScoringResultParserTests : public Test {};

TEST_F(ScoringResultParserTests, Successful_AC) {
    EXPECT_THAT(ScoringResultParser::parse(new istringstream("AC\n")), Eq(
            ScoringResultBuilder().setVerdict(Verdict::ac()).build()));
}

TEST_F(ScoringResultParserTests, Successful_WA) {
    EXPECT_THAT(ScoringResultParser::parse(new istringstream("WA\n")), Eq(
            ScoringResultBuilder().setVerdict(Verdict::wa()).build()));
}

TEST_F(ScoringResultParserTests, Failed_ERR_Empty) {
    EXPECT_THAT(ScoringResultParser::parse(new istringstream("")), Eq(
            ScoringResultBuilder()
                    .setVerdict(Verdict::err())
                    .setPrivateInfo("Expected: <verdict> on the first line")
                    .build()));
}

TEST_F(ScoringResultParserTests, Failed_ERR_UnknownVerdict) {
    EXPECT_THAT(ScoringResultParser::parse(new istringstream("hokus pokus")), Eq(
            ScoringResultBuilder()
                    .setVerdict(Verdict::err())
                    .setPrivateInfo("Unknown verdict: hokus pokus")
                    .build()));
}

}

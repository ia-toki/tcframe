#include "gtest/gtest.h"

#include "tcframe/deprecated/ioformat/IOFormatProvider.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using tcframe_old::IOFormatProvider;

TEST(DeprecatedIOFormatProviderTest, Printing) {
    int A = 1;
    int B = 2;
    int K = 77;

    IOFormatProvider provider;

    ostringstream sout;

    provider.beginPrintingFormat(&sout);
    provider.applyLastSegment(); provider.applyLineSegment("A, B"), A, B;
    provider.applyLastSegment(); provider.applyLineSegment("K"), K;
    provider.endPrintingFormat();

    EXPECT_EQ("1 2\n77\n", sout.str());
}

TEST(DeprecatedIOFormatProviderTest, Parsing) {
    int A, B;
    int K;

    IOFormatProvider provider;

    istringstream sin("1 2\n77\n");

    provider.beginParsingFormat(&sin);
    provider.applyLastSegment(); provider.applyLineSegment("A, B"), A, B;
    provider.applyLastSegment(); provider.applyLineSegment("K"), K;
    provider.endPrintingFormat();

    EXPECT_EQ(1, A);
    EXPECT_EQ(2, B);
    EXPECT_EQ(77, K);
}

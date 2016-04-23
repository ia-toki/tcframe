#include "gmock/gmock.h"
#include "../mock.hpp"

#include <sstream>

#include "../type/MockScalar.hpp"
#include "MockWhitespacePrinter.hpp"
#include "tcframe/format/LineIOSegmentPrinter.hpp"

using ::testing::InSequence;
using ::testing::Test;

using std::ostringstream;

namespace tcframe {

class LineIOSegmentPrinterTests : public Test {
protected:
    Mock(Scalar) scalarA;
    Mock(Scalar) scalarB;
    Mock(Scalar) scalarC;
    Mock(WhitespacePrinter) whitespacePrinter;
    ostream* out = new ostringstream();

    LineIOSegmentPrinter printer = LineIOSegmentPrinter(&whitespacePrinter);
};

TEST_F(LineIOSegmentPrinterTests, ScalarsPrinting) {
    LineIOSegment* segment = LineIOSegmentBuilder()
            .addScalarVariable(&scalarA)
            .addScalarVariable(&scalarB)
            .addScalarVariable(&scalarC)
            .build();

    {
        InSequence sequence;
        EXPECT_CALL(scalarA, printTo(out));
        EXPECT_CALL(whitespacePrinter, printSpace(out))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarB, printTo(out));
        EXPECT_CALL(whitespacePrinter, printSpace(out))
                .RetiresOnSaturation();
        EXPECT_CALL(scalarC, printTo(out));
        EXPECT_CALL(whitespacePrinter, printNewline(out));
    }

    printer.print(segment, out);
}

}

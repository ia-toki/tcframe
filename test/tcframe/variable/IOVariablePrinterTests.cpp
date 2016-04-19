#include "gmock/gmock.h"

#include <ostream>
#include <sstream>

#include "tcframe/io/IOFormat.hpp"
#include "tcframe/io/IOSegment.hpp"
#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/variable/IOVariablesPrinter.hpp"

#include "../io/FakeIOSegment.hpp"
#include "../io/MockLineIOSegmentPrinter.hpp"

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Test;

using std::ostream;
using std::ostringstream;

namespace tcframe {

class IOVariablePrinterTests : public Test {
protected:
    IOSegment* segmentA = new FakeIOSegment(IOSegmentType::LINE);
    IOSegment* segmentB = new FakeIOSegment(IOSegmentType::LINE);

    const IOFormat ioFormat = IOFormatBuilder()
            .prepareForInputFormat()
            .addIOSegment(segmentA)
            .addIOSegment(segmentB)
            .build();

    ostream* out = new ostringstream();

    NiceMock<MockLineIOSegmentPrinter> lineIOSegmentPrinter;

    IOVariablesPrinter printer;

    IOVariablePrinterTests()
            : printer(&lineIOSegmentPrinter, ioFormat)
    {}
};

TEST_F(IOVariablePrinterTests, CanPrint) {
    {
        InSequence sequence;

        EXPECT_CALL(lineIOSegmentPrinter, print(reinterpret_cast<LineIOSegment*>(segmentA), out));
        EXPECT_CALL(lineIOSegmentPrinter, print(reinterpret_cast<LineIOSegment*>(segmentB), out));
    }

    printer.printInput(out);
}

}

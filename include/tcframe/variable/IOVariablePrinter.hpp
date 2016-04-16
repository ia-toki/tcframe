#pragma once

#include <iostream>
#include <ostream>
#include <vector>

#include "tcframe/io/IOFormat.hpp"
#include "tcframe/io/IOSegmentVisitor.hpp"
#include "tcframe/io/LineIOSegment.hpp"

using std::endl;
using std::ostream;
using std::vector;

namespace tcframe {

class IOVariablePrinter {
private:
    IOFormat ioFormat_;

public:
    virtual ~IOVariablePrinter() {}

    IOVariablePrinter(const IOFormat& ioFormat)
            : ioFormat_(ioFormat)
    {}

    void printInput(ostream* out) {
        IOSegmentPrinter printer(out);
        print(printer, ioFormat_.inputFormat());
    }

private:
    class IOSegmentPrinter;

    void print(IOSegmentPrinter& printer, const vector<IOSegment*>& segments) {
        for (IOSegment* segment : segments) {
            segment->accept(&printer);
        }
    }

    class IOSegmentPrinter : public IOSegmentVisitor {
    private:
        ostream* out_;

    public:
        IOSegmentPrinter(ostream *out)
                : out_(out)
        {}

        void visit(LineIOSegment* segment) const {
            bool first = true;
            for (LineIOSegmentVariable* variable : segment->variables()) {
                if (auto scalarVariable = dynamic_cast<LineIOSegmentScalarVariable*>(variable)) {
                    if (!first) {
                        *out_ << " ";
                    }
                    first = false;

                    scalarVariable->variable()->printTo(*out_);
                }
            }
            *out_ << endl;
        }
    };
};

}

#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "GridIOSegmentManipulator.hpp"
#include "LineIOSegmentManipulator.hpp"
#include "LinesIOSegmentManipulator.hpp"
#include "RawLineIOSegmentManipulator.hpp"
#include "RawLinesIOSegmentManipulator.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"

using std::istream;
using std::move;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class IOManipulator {
private:
    IOFormat ioFormat_;

    GridIOSegmentManipulator gridIOSegmentManipulator;
    LineIOSegmentManipulator lineIOSegmentManipulator;
    LinesIOSegmentManipulator linesIOSegmentManipulator;
    RawLineIOSegmentManipulator rawLineIOSegmentManipulator;
    RawLinesIOSegmentManipulator rawLinesIOSegmentManipulator;

public:
    virtual ~IOManipulator() = default;

    explicit IOManipulator(IOFormat ioFormat)
            : ioFormat_(move(ioFormat)) {}

    virtual void printInput(ostream* out) {
        print(ioFormat_.inputFormat(), out);
    }

    virtual void parseInput(istream* in) {
        parse(ioFormat_.inputFormat(), in);
    }

    virtual void parseOutput(istream* in) {
        if (!ioFormat_.outputFormats().empty()) {
            ioFormat_.beforeOutputFormat()();

            long long initialPos = in->tellg();
            bool successful = false;
            string errorMessage;
            for (const IOSegments& outputFormat : ioFormat_.outputFormats()) {
                try {
                    parse(outputFormat, in);
                    successful = true;
                    break;
                } catch (runtime_error& e) {
                    in->clear();
                    in->seekg(initialPos);
                    if (errorMessage.empty()) {
                        errorMessage = e.what();
                    }
                }
            }

            if (!successful) {
                if (ioFormat_.outputFormats().size() == 1) {
                    throw runtime_error(errorMessage);
                }
                throw runtime_error("Test case output does not conform to any of the output formats");
            }
        }
    }

private:
    void print(const IOSegments& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                gridIOSegmentManipulator.print((GridIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINE) {
                lineIOSegmentManipulator.print((LineIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINES) {
                linesIOSegmentManipulator.print((LinesIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::RAW_LINE) {
                rawLineIOSegmentManipulator.print((RawLineIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::RAW_LINES) {
                rawLinesIOSegmentManipulator.print((RawLinesIOSegment*) segment, out);
            }
        }
    }

    void parse(const IOSegments& segments, istream* in) {
        string lastVariableName;
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                lastVariableName = gridIOSegmentManipulator.parse((GridIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINE) {
                lastVariableName = lineIOSegmentManipulator.parse((LineIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINES) {
                lastVariableName = linesIOSegmentManipulator.parse((LinesIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::RAW_LINE) {
                lastVariableName = rawLineIOSegmentManipulator.parse((RawLineIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::RAW_LINES) {
                lastVariableName = rawLinesIOSegmentManipulator.parse((RawLinesIOSegment*) segment, in);
            }
        }
        if (!lastVariableName.empty()) {
            WhitespaceManipulator::ensureEof(in, lastVariableName);
        } else {
            WhitespaceManipulator::ensureEof(in);
        }
    }
};

}

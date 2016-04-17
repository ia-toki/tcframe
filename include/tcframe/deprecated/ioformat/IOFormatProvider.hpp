#pragma once

#include "tcframe/deprecated/ioformat/GridIOSegment.hpp"
#include "tcframe/deprecated/ioformat/IOSegment.hpp"
#include "tcframe/deprecated/ioformat/LineIOSegment.hpp"
#include "tcframe/deprecated/ioformat/LinesIOSegment.hpp"

#include <cctype>
#include <istream>
#include <ostream>
#include <string>

using std::char_traits;
using std::istream;
using std::ostream;
using std::string;

namespace tcframe_old {

class IOFormatProvider {
public:
    void beginParsingFormat(istream* in) {
        this->in = in;
        this->out = nullptr;
        this->lastSegment = nullptr;
    }

    void beginPrintingFormat(ostream* out) {
        this->in = nullptr;
        this->out = out;
        this->lastSegment = nullptr;
    }

    LineIOSegmentOld& applyLineSegment(string names) {
        LineIOSegmentOld* segment = new LineIOSegmentOld(names);
        lastSegment = segment;
        return *segment;
    }

    LinesIOSegmentOld& applyLinesSegment(string names) {
        LinesIOSegmentOld* segment = new LinesIOSegmentOld(names);
        lastSegment = segment;
        return *segment;
    }

    GridIOSegmentOld& applyGridSegment(string name) {
        GridIOSegmentOld* segment = new GridIOSegmentOld(name);
        lastSegment = segment;
        return *segment;
    }

    void endParsingFormat() {
        applyLastSegment();

        if (in->get() != char_traits<char>::eof()) {
            throw ParsingException("Expected: <EOF>");
        }
    }

    void endPrintingFormat() {
        applyLastSegment();
    }

    void applyLastSegment() {
        if (lastSegment == nullptr) {
            return;
        }

        if (in != nullptr) {
            lastSegment->parseFrom(*in);
        } else {
            lastSegment->printTo(*out);
        }
    }

private:
    istream* in;
    ostream* out;
    IOSegment* lastSegment;
};

}

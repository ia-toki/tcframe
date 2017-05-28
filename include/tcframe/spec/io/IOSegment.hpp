#pragma once

#include <climits>

#include <vector>

using std::vector;

namespace tcframe {

const int NO_SIZE = INT_MIN;

enum class IOSegmentType {
    GRID,
    LINE,
    LINES,
    RAW_LINE,
    RAW_LINES
};

struct IOSegment {
public:
    virtual ~IOSegment() {}

    virtual IOSegmentType type() const = 0;

    virtual bool equals(IOSegment* o) const = 0;
};

class IOSegmentBuilder {
public:
    virtual ~IOSegmentBuilder() {}

    virtual IOSegment* build() = 0;
};

using IOSegments = vector<IOSegment*>;

}

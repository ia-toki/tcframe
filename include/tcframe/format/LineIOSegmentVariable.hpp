#pragma once

namespace tcframe {

enum LineIOSegmentVariableType {
    SCALAR
};

struct LineIOSegmentVariable {
public:
    virtual ~LineIOSegmentVariable() {}

    virtual LineIOSegmentVariableType type() = 0;
};

}

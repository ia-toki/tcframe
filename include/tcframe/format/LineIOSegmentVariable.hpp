#pragma once

namespace tcframe {

enum LineIOSegmentVariableType {
    SCALAR
};

struct LineIOSegmentVariable {
public:
    virtual ~LineIOSegmentVariable() {}

    virtual LineIOSegmentVariableType type() const = 0;

    bool operator==(const LineIOSegmentVariable& o) const {
        return type() == o.type() && equals(o);
    }

protected:
    virtual bool equals(const LineIOSegmentVariable& o) const = 0;
};

}

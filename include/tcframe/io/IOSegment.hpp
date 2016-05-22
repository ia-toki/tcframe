#pragma once

namespace tcframe {

enum IOSegmentType {
    LINE
};

struct IOSegment {
public:
    virtual ~IOSegment() {}

    virtual IOSegmentType type() const = 0;

    bool operator==(const IOSegment& o) const {
        return type() == o.type() && equals(o);
    }

protected:
    virtual bool equals(const IOSegment& o) const = 0;
};

class IOSegmentBuilder {
public:
    virtual ~IOSegmentBuilder() {}

    virtual IOSegment* build() = 0;
};

}

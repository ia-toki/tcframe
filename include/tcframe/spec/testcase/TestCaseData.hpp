#pragma once

namespace tcframe {

enum class TestCaseDataType {
    SAMPLE,
    OFFICIAL
};

struct TestCaseData {
public:
    virtual ~TestCaseData() = default;

    virtual TestCaseDataType type() const = 0;

    virtual bool equals(TestCaseData *o) const = 0;
};

}

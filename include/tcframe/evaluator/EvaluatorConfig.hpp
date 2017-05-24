#pragma once

#include <utility>

using std::move;

namespace tcframe {

enum class TestCaseOutputType {
    OPTIONAL,
    NOT_REQUIRED
};

class EvaluatorConfig {
    friend class EvaluatorConfigBuilder;

private:
    TestCaseOutputType testCaseOutputType_;

public:
    TestCaseOutputType testCaseOutputType() const {
        return testCaseOutputType_;
    }

    bool operator==(const EvaluatorConfig& o) const {
        return tie(testCaseOutputType_) == tie(o.testCaseOutputType_);
    }
};

class EvaluatorConfigBuilder {
private:
    EvaluatorConfig subject_;

public:
    EvaluatorConfigBuilder& setTestCaseOutputType(TestCaseOutputType testCaseOutputType) {
        subject_.testCaseOutputType_ = testCaseOutputType;
        return *this;
    }

    EvaluatorConfig build() {
        return move(subject_);
    }
};

}

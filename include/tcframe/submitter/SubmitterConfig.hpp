#pragma once

#include <string>
#include <utility>

#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;

namespace tcframe {

class SubmitterConfig {
    friend class SubmitterConfigBuilder;

private:
    bool hasMultipleTestCasesCount_;
    string slug_;
    string solutionCommand_;
    string testCasesDir_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    bool hasMultipleTestCasesCount() const {
        return hasMultipleTestCasesCount_;
    }

    const string& slug() const {
        return slug_;
    }

    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const string& testCasesDir() const {
        return testCasesDir_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const SubmitterConfig& o) const {
        return tie(hasMultipleTestCasesCount_, slug_, solutionCommand_, testCasesDir_, timeLimit_, memoryLimit_) ==
                tie(o.hasMultipleTestCasesCount_,o.slug_, o.solutionCommand_, o.testCasesDir_, o.timeLimit_,
                    o.memoryLimit_);
    }
};

class SubmitterConfigBuilder {
private:
    SubmitterConfig subject_;

public:
    SubmitterConfigBuilder(const SubmitterConfig& from)
            : subject_(from) {}

    SubmitterConfigBuilder() {
        subject_.hasMultipleTestCasesCount_ = false;
        subject_.solutionCommand_ = DefaultValues::solutionCommand();
        subject_.testCasesDir_ = DefaultValues::testCasesDir();
    }

    SubmitterConfigBuilder& setHasMultipleTestCasesCount(bool hasMultipleTestCasesCount) {
        subject_.hasMultipleTestCasesCount_ = hasMultipleTestCasesCount;
        return *this;
    }

    SubmitterConfigBuilder& setSlug(string slug) {
        subject_.slug_ = slug;
        return *this;
    }

    SubmitterConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    SubmitterConfigBuilder& setTestCasesDir(string testCasesDir) {
        subject_.testCasesDir_ = testCasesDir;
        return *this;
    }

    SubmitterConfigBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    SubmitterConfigBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    SubmitterConfig build() {
        return move(subject_);
    }
};

}

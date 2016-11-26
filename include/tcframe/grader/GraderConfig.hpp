#pragma once

#include <string>
#include <utility>

#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct GraderConfig {
    friend class GraderConfigBuilder;

private:
    string slug_;

    bool hasMultipleTestCases_;
    string solutionCommand_;
    string outputDir_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const string& slug() const {
        return slug_;
    }

    bool hasMultipleTestCases() const {
        return hasMultipleTestCases_;
    }

    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const string& outputDir() const {
        return outputDir_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const GraderConfig& o) const {
        return tie(hasMultipleTestCases_, slug_, solutionCommand_, outputDir_, timeLimit_, memoryLimit_) ==
                tie(o.hasMultipleTestCases_,o.slug_, o.solutionCommand_, o.outputDir_, o.timeLimit_,
                    o.memoryLimit_);
    }
};

class GraderConfigBuilder {
private:
    GraderConfig subject_;

public:
    GraderConfigBuilder(const GraderConfig& from)
            : subject_(from) {}

    GraderConfigBuilder(string slug) {
        subject_.slug_ = slug;
        subject_.hasMultipleTestCases_ = false;
        subject_.solutionCommand_ = CommonConfig::solutionCommand();
        subject_.outputDir_ = CommonConfig::outputDir();
    }

    GraderConfigBuilder& setHasMultipleTestCases(optional<bool> hasMultipleTestCases) {
        if (hasMultipleTestCases) {
            setHasMultipleTestCases(hasMultipleTestCases.value());
        }
        return *this;
    }

    GraderConfigBuilder& setHasMultipleTestCases(bool hasMultipleTestCases) {
        subject_.hasMultipleTestCases_ = hasMultipleTestCases;
        return *this;
    }

    GraderConfigBuilder& setSolutionCommand(optional<string> solutionCommand) {
        if (solutionCommand) {
            setSolutionCommand(solutionCommand.value());
        }
        return *this;
    }

    GraderConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    GraderConfigBuilder& setOutputDir(optional<string> outputDir) {
        if (outputDir) {
            setOutputDir(outputDir.value());
        }
        return *this;
    }

    GraderConfigBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = outputDir;
        return *this;
    }

    GraderConfigBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    GraderConfigBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    GraderConfig build() {
        return move(subject_);
    }
};

}

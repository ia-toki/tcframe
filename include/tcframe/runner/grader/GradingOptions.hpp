#pragma once

#include <string>
#include <utility>
#include <vector>

#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;
using std::vector;

namespace tcframe {

struct GradingOptions {
    friend class GradingOptionsBuilder;

private:
    string slug_;
    vector<double> subtaskPoints_;
    string solutionCommand_;
    string outputDir_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const string& slug() const {
        return slug_;
    }

    const vector<double>& subtaskPoints() const {
        return subtaskPoints_;
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

    bool operator==(const GradingOptions& o) const {
        return tie(slug_, solutionCommand_, outputDir_, timeLimit_, memoryLimit_) ==
                tie(o.slug_, o.solutionCommand_, o.outputDir_, o.timeLimit_, o.memoryLimit_);
    }
};

class GradingOptionsBuilder {
private:
    GradingOptions subject_;

public:
    GradingOptionsBuilder(const GradingOptions& from)
            : subject_(from) {}

    GradingOptionsBuilder(string slug) {
        subject_.slug_ = slug;
    }

    GradingOptionsBuilder& setSubtaskPoints(vector<double> subtaskPoints) {
        subject_.subtaskPoints_ = subtaskPoints;
        return *this;
    }

    GradingOptionsBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    GradingOptionsBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = outputDir;
        return *this;
    }

    GradingOptionsBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    GradingOptionsBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    GradingOptions build() {
        return move(subject_);
    }
};

}

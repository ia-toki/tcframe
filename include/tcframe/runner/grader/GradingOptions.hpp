#pragma once

#include <string>
#include <utility>
#include <vector>

#include "tcframe/runner/evaluator/SolutionMap.hpp"
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
    SolutionMap solutions_;
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

    const SolutionMap& solutions() const {
        return solutions_;
    }

    string solutionCommand() const {
        return solutions_.defaultCommand();
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
        return tie(slug_, subtaskPoints_, solutions_, outputDir_, timeLimit_, memoryLimit_) ==
                tie(o.slug_, o.subtaskPoints_, o.solutions_, o.outputDir_, o.timeLimit_, o.memoryLimit_);
    }
};

class GradingOptionsBuilder {
private:
    GradingOptions subject_;

public:
    explicit GradingOptionsBuilder(GradingOptions from)
            : subject_(move(from)) {}

    explicit GradingOptionsBuilder(string slug) {
        subject_.slug_ = move(slug);
    }

    GradingOptionsBuilder& setSubtaskPoints(vector<double> subtaskPoints) {
        subject_.subtaskPoints_ = move(subtaskPoints);
        return *this;
    }

    GradingOptionsBuilder& setSolutions(SolutionMap solutions) {
        subject_.solutions_ = move(solutions);
        return *this;
    }

    GradingOptionsBuilder& setSolutionCommand(const string& solutionCommand) {
        subject_.solutions_.set(SolutionMap::DEFAULT_KEY, solutionCommand);
        return *this;
    }

    GradingOptionsBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = move(outputDir);
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

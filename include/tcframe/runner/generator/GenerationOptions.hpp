#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/runner/evaluator/SolutionMap.hpp"
#include "tcframe/spec/core.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct GenerationOptions {
    friend class GenerationOptionsBuilder;

private:
    string slug_;
    unsigned seed_;
    SolutionMap solutions_;
    string outputDir_;
    bool hasTcOutput_;

public:
    const string& slug() const {
        return slug_;
    }

    unsigned int seed() const {
        return seed_;
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

    bool hasTcOutput() const {
        return hasTcOutput_;
    }

    bool operator==(const GenerationOptions& o) const {
        return tie(slug_, seed_, solutions_, outputDir_, hasTcOutput_) ==
                tie(o.slug_, o.seed_, o.solutions_, o.outputDir_, o.hasTcOutput_);
    }
};

class GenerationOptionsBuilder {
private:
    GenerationOptions subject_;

public:
    explicit GenerationOptionsBuilder(GenerationOptions from)
            : subject_(move(from)) {}

    explicit GenerationOptionsBuilder(string slug) {
        subject_.slug_ = move(slug);
    }

    GenerationOptionsBuilder& setSeed(unsigned seed) {
        subject_.seed_ = seed;
        return *this;
    }

    GenerationOptionsBuilder& setSolutions(SolutionMap solutions) {
        subject_.solutions_ = move(solutions);
        return *this;
    }

    GenerationOptionsBuilder& setSolutionCommand(const string& solutionCommand) {
        subject_.solutions_.set(SolutionMap::DEFAULT_KEY, solutionCommand);
        return *this;
    }

    GenerationOptionsBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = move(outputDir);
        return *this;
    }

    GenerationOptionsBuilder& setHasTcOutput(bool hasTcOutput) {
        subject_.hasTcOutput_ = hasTcOutput;
        return *this;
    }

    GenerationOptions build() {
        return move(subject_);
    }
};

}

#pragma once

#include <string>
#include <tuple>
#include <utility>

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
    string solutionCommand_;
    string outputDir_;
    bool hasTcOutput_;

public:
    const string& slug() const {
        return slug_;
    }

    unsigned int seed() const {
        return seed_;
    }

    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const string& outputDir() const {
        return outputDir_;
    }

    bool hasTcOutput() const {
        return hasTcOutput_;
    }

    bool operator==(const GenerationOptions& o) const {
        return tie(slug_, seed_, solutionCommand_, outputDir_, hasTcOutput_) ==
                tie(o.slug_, o.seed_, o.solutionCommand_, o.outputDir_, o.hasTcOutput_);
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

    GenerationOptionsBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = move(solutionCommand);
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

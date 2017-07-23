#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"
#include "tcframe/spec/core.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct GenerationOptions {
    friend class GenerationOptionsBuilder;

private:
    string slug_;

    int* multipleTestCasesCounter_;
    optional<string> multipleTestCasesOutputPrefix_;
    optional<string> multipleTestCasesFirstOutputPrefix_;
    unsigned seed_;
    string solutionCommand_;
    string outputDir_;
    bool needsOutput_;

public:
    const string& slug() const {
        return slug_;
    }

    int* multipleTestCasesCounter() const {
        return multipleTestCasesCounter_;
    }

    const optional<string>& multipleTestCasesOutputPrefix() const {
        return multipleTestCasesOutputPrefix_;
    }

    const optional<string>& multipleTestCasesFirstOutputPrefix() const {
        return multipleTestCasesFirstOutputPrefix_;
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

    bool needsOutput() const {
        return needsOutput_;
    }

    bool operator==(const GenerationOptions& o) const {
        return tie(slug_, multipleTestCasesCounter_, multipleTestCasesOutputPrefix_,
                   seed_, solutionCommand_, outputDir_, needsOutput_) ==
                tie(o.slug_, o.multipleTestCasesCounter_, o.multipleTestCasesOutputPrefix_,
                    o.seed_, o.solutionCommand_, o.outputDir_, o.needsOutput_);
    }
};

class GenerationOptionsBuilder {
private:
    GenerationOptions subject_;

public:
    GenerationOptionsBuilder(const GenerationOptions& from)
            : subject_(from) {}

    GenerationOptionsBuilder(string slug) {
        subject_.slug_ = slug;
        subject_.multipleTestCasesCounter_ = nullptr;
    }

    GenerationOptionsBuilder& setMultipleTestCasesCounter(optional<int*> counter) {
        if (counter) {
            setMultipleTestCasesCounter(counter.value());
        }
        return *this;
    }

    GenerationOptionsBuilder& setMultipleTestCasesCounter(int* counter) {
        subject_.multipleTestCasesCounter_ = counter;
        return *this;
    }

    GenerationOptionsBuilder& setMultipleTestCasesOutputPrefix(optional<string> outputPrefix) {
        if (outputPrefix) {
            setMultipleTestCasesOutputPrefix(outputPrefix.value());
        }
        return *this;
    }

    GenerationOptionsBuilder& setMultipleTestCasesOutputPrefix(string outputPrefix) {
        subject_.multipleTestCasesOutputPrefix_ = outputPrefix;
        subject_.multipleTestCasesFirstOutputPrefix_ = StringUtils::interpolate(outputPrefix, 1);
        return *this;
    }

    GenerationOptionsBuilder& setSeed(unsigned seed) {
        subject_.seed_ = seed;
        return *this;
    }

    GenerationOptionsBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    GenerationOptionsBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = outputDir;
        return *this;
    }

    GenerationOptionsBuilder& setNeedsOutput(bool needsOutput) {
        subject_.needsOutput_ = needsOutput;
        return *this;
    }

    GenerationOptions build() {
        return move(subject_);
    }
};

}

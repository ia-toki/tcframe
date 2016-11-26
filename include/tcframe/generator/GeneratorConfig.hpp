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

struct GeneratorConfig {
    friend class GeneratorConfigBuilder;

private:
    string slug_;

    int* multipleTestCasesCounter_;
    optional<string> multipleTestCasesOutputPrefix_;
    optional<string> multipleTestCasesFirstOutputPrefix_;
    unsigned seed_;
    string solutionCommand_;
    string outputDir_;

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

    bool operator==(const GeneratorConfig& o) const {
        return tie(slug_, multipleTestCasesCounter_, multipleTestCasesOutputPrefix_,
                   seed_, solutionCommand_, outputDir_) ==
                tie(o.slug_, o.multipleTestCasesCounter_, o.multipleTestCasesOutputPrefix_,
                    o.seed_, o.solutionCommand_, o.outputDir_);
    }
};

class GeneratorConfigBuilder {
private:
    GeneratorConfig subject_;

public:
    GeneratorConfigBuilder(const GeneratorConfig& from)
            : subject_(from) {}

    GeneratorConfigBuilder(string slug) {
        subject_.slug_ = slug;
        subject_.multipleTestCasesCounter_ = nullptr;
        subject_.seed_ = CommonConfig::seed();
        subject_.solutionCommand_ = CommonConfig::solutionCommand();
        subject_.outputDir_ = CommonConfig::outputDir();
    }

    GeneratorConfigBuilder& setMultipleTestCasesCounter(optional<int*> counter) {
        if (counter) {
            setMultipleTestCasesCounter(counter.value());
        }
        return *this;
    }

    GeneratorConfigBuilder& setMultipleTestCasesCounter(int* counter) {
        subject_.multipleTestCasesCounter_ = counter;
        return *this;
    }

    GeneratorConfigBuilder& setMultipleTestCasesOutputPrefix(optional<string> outputPrefix) {
        if (outputPrefix) {
            setMultipleTestCasesOutputPrefix(outputPrefix.value());
        }
        return *this;
    }

    GeneratorConfigBuilder& setMultipleTestCasesOutputPrefix(string outputPrefix) {
        subject_.multipleTestCasesOutputPrefix_ = outputPrefix;
        subject_.multipleTestCasesFirstOutputPrefix_ = StringUtils::interpolate(outputPrefix, 1);
        return *this;
    }

    GeneratorConfigBuilder& setSeed(optional<unsigned> seed) {
        if (seed) {
            setSeed(seed.value());
        }
        return *this;
    }

    GeneratorConfigBuilder& setSeed(unsigned seed) {
        subject_.seed_ = seed;
        return *this;
    }

    GeneratorConfigBuilder& setSolutionCommand(optional<string> solutionCommand) {
        if (solutionCommand) {
            setSolutionCommand(solutionCommand.value());
        }
        return *this;
    }

    GeneratorConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
        return *this;
    }

    GeneratorConfigBuilder& setOutputDir(optional<string> outputDir) {
        if (outputDir) {
            setOutputDir(outputDir.value());
        }
        return *this;
    }

    GeneratorConfigBuilder& setOutputDir(string outputDir) {
        subject_.outputDir_ = outputDir;
        return *this;
    }

    GeneratorConfig build() {
        return move(subject_);
    }
};

}

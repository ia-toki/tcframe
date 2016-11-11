#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <utility>

#include "tcframe/spec/core.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct GeneratorConfig {
    friend class GeneratorConfigBuilder;

private:
    int* multipleTestCasesCount_;
    unsigned seed_;
    string slug_;
    string solutionCommand_;
    string outputDir_;

public:
    int* multipleTestCasesCount() const {
        return multipleTestCasesCount_;
    }

    unsigned int seed() const {
        return seed_;
    }

    const string& slug() const {
        return slug_;
    }

    const string& solutionCommand() const {
        return solutionCommand_;
    }

    const string& outputDir() const {
        return outputDir_;
    }

    bool operator==(const GeneratorConfig& o) const {
        return tie(multipleTestCasesCount_, seed_, slug_, solutionCommand_, outputDir_) ==
                tie(o.multipleTestCasesCount_, o.seed_, o.slug_, o.solutionCommand_, o.outputDir_);
    }
};

class GeneratorConfigBuilder {
private:
    GeneratorConfig subject_;

public:
    GeneratorConfigBuilder(const GeneratorConfig& from)
            : subject_(from) {}

    GeneratorConfigBuilder() {
        subject_.multipleTestCasesCount_ = nullptr;
    }

    GeneratorConfigBuilder& setMultipleTestCasesCount(int* var) {
        subject_.multipleTestCasesCount_ = var;
        return *this;
    }

    GeneratorConfigBuilder& setSeed(unsigned seed) {
        subject_.seed_ = seed;
        return *this;
    }

    GeneratorConfigBuilder& setSlug(string slug) {
        subject_.slug_ = slug;
        return *this;
    }

    GeneratorConfigBuilder& setSolutionCommand(string solutionCommand) {
        subject_.solutionCommand_ = solutionCommand;
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

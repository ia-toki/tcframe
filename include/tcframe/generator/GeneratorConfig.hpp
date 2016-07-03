#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/core.hpp"

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
    string testCasesDir_;

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

    const string& testCasesDir() const {
        return testCasesDir_;
    }

    bool operator==(const GeneratorConfig& o) const {
        return tie(multipleTestCasesCount_, seed_, slug_, solutionCommand_, testCasesDir_) ==
                tie(o.multipleTestCasesCount_, o.seed_, o.slug_, o.solutionCommand_, o.testCasesDir_);
    }
};

class GeneratorConfigBuilder {
private:
    GeneratorConfig subject_;

public:
    GeneratorConfigBuilder() {
        subject_.multipleTestCasesCount_ = nullptr;
        subject_.seed_ = DefaultValues::seed();
        subject_.slug_ = DefaultValues::slug();
        subject_.solutionCommand_ = DefaultValues::solutionCommand();
        subject_.testCasesDir_ = DefaultValues::testCasesDir();
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

    GeneratorConfigBuilder& setTestCasesDir(string testCasesDir) {
        subject_.testCasesDir_ = testCasesDir;
        return *this;
    }

    GeneratorConfig build() {
        return move(subject_);
    }
};

}

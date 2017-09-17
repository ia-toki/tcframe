#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "tcframe/runner/logger.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/verifier.hpp"
#include "tcframe/util.hpp"

using std::pair;
using std::string;
using std::vector;

namespace tcframe {

class GeneratorLogger : public virtual BaseLogger {
public:
    virtual ~GeneratorLogger() = default;

    virtual void logIntroduction() = 0;
    virtual void logSuccessfulResult() = 0;
    virtual void logFailedResult() = 0;
    virtual void logTestCaseSuccessfulResult() = 0;
    virtual void logTestCaseFailedResult(const optional<string>& testCaseDescription) = 0;
    virtual void logMultipleTestCasesCombinationIntroduction(const string& testGroupName) = 0;
    virtual void logMultipleTestCasesCombinationSuccessfulResult() = 0;
    virtual void logMultipleTestCasesCombinationFailedResult() = 0;
};

}

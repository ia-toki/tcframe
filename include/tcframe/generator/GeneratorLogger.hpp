#pragma once

#include <string>

#include "tcframe/logger.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verifier.hpp"

using std::string;

namespace tcframe {

class GeneratorLogger : public virtual BaseLogger {
public:
    virtual ~GeneratorLogger() {}

    virtual void logIntroduction() = 0;
    virtual void logSuccessfulResult() = 0;
    virtual void logFailedResult() = 0;
    virtual void logTestCaseSuccessfulResult() = 0;
    virtual void logTestCaseFailedResult(const optional<string>& testCaseDescription) = 0;
    virtual void logMultipleTestCasesCombinationIntroduction(const string& testGroupName) = 0;
    virtual void logMultipleTestCasesCombinationSuccessfulResult() = 0;
    virtual void logMultipleTestCasesCombinationFailedResult() = 0;
    virtual void logConstraintsVerificationFailure(const ConstraintsVerificationResult& result) = 0;
    virtual void logMultipleTestCasesConstraintsVerificationFailure(
            const MultipleTestCasesConstraintsVerificationResult& result) = 0;
    virtual void logSampleTestCaseCheckFailure() = 0;
    virtual void logSampleTestCaseNoOutputNeededFailure() = 0;
    virtual void logSimpleFailure(const string& message) = 0;
 };

}

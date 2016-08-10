#pragma once

#include <streambuf>
#include <string>

#include "Scorer.hpp"
#include "Verdict.hpp"
#include "SubmitterLogger.hpp"
#include "tcframe/os.hpp"

using std::istreambuf_iterator;
using std::string;

namespace tcframe {

class DiffScorer : public Scorer {
private:
    OperatingSystem* os_;
    SubmitterLogger* logger_;

public:
    DiffScorer(OperatingSystem* os, SubmitterLogger* logger)
            : os_(os)
            , logger_(logger) {}

    Verdict score(const TestCase& testCase, const SubmitterConfig& config) {
        string outputFilename = config.testCasesDir() + "/" + testCase.id() + ".out";

        string briefDiffCommand = "diff --brief _evaluation.out " + outputFilename;
        ExecutionResult briefResult = os_->execute(ExecutionRequestBuilder().setCommand(briefDiffCommand).build());

        if (briefResult.info().isSuccessful()) {
            logger_->logTestCaseVerdict(Verdict::ac());
            return Verdict::ac();
        }

        logger_->logTestCaseVerdict(Verdict::wa());

        string diffCommand = string() +
                 "diff " +
                 "--unchanged-line-format=' %.2dn    %L' " +
                 "--old-line-format='(expected) [line %.2dn]    %L' " +
                 "--new-line-format='(received) [line %.2dn]    %L' " +
                 outputFilename + " _evaluation.out";
        string scoringCommand = diffCommand + " | head -n 10";
        ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename("_diff.out")
                .build());

        string diff = string(istreambuf_iterator<char>(*result.outputStream()), istreambuf_iterator<char>());
        logger_->logDiffFailure(diff);

        return Verdict::wa();
    }
};

}

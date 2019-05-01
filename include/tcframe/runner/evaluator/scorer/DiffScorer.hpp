#pragma once

#include <istream>
#include <string>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::istream;
using std::string;

namespace tcframe {

class DiffScorer : public Scorer {
private:
    OperatingSystem* os_;

public:
    virtual ~DiffScorer() = default;

    explicit DiffScorer(OperatingSystem* os)
            : os_(os) {}

    ScoringResult score(const string&, const string& outputFilename, const string& evaluationFilename) {
        string scoringCommand = string()
                + "diff --brief " + outputFilename + " " + evaluationFilename + " > /dev/null"
                + " || "
                + "( "
                + "echo 'Diff:'; "
                + "diff "
                + "--unchanged-line-format=' %.2dn    %L' "
                + "--old-line-format='(expected) [line %.2dn]    %L' "
                + "--new-line-format='(received) [line %.2dn]    %L' "
                + outputFilename + " " + evaluationFilename
                + " | head -n 10"
                + " )";

        ExecutionResult executionResult = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename(SCORING_OUT_FILENAME)
                .build());
        istream* output = os_->openForReading(SCORING_OUT_FILENAME);
        string diff = StringUtils::streamToString(output);
        os_->closeOpenedStream(output);

        TestCaseVerdict verdict;
        if (diff.empty()) {
            verdict = TestCaseVerdict(Verdict::ac());
        } else {
            verdict = TestCaseVerdict(Verdict::wa());
        }

        // TODO(fushar): Figure out how to directly output the diff to the standard error.
        auto newExecutionResult = ExecutionResultBuilder(executionResult)
                .setStandardError(diff)
                .build();

        return {verdict, newExecutionResult};
    }
};

}

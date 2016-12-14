#pragma once

#include <string>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::string;

namespace tcframe {

class DiffScorer : public Scorer {
private:
    OperatingSystem* os_;

public:
    virtual ~DiffScorer() {}

    DiffScorer(OperatingSystem* os)
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

        ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename("_diff.out")
                .build());
        string message = StringUtils::streamToString(result.outputStream());

        Verdict verdict = message.empty()
                ? Verdict::ac()
                : Verdict::wa();

        return ScoringResultBuilder()
                .setExecutionResult(result)
                .setVerdict(verdict)
                .setMessage(message)
                .build();
    }
};

}

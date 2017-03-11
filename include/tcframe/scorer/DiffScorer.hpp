#pragma once

#include <string>

#include "Scorer.hpp"
#include "tcframe/grade.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

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

        ExecutionResult executionResult = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename(SCORING_FILENAME)
                .build());
        istream* output = os_->openForReading(SCORING_FILENAME);
        string privateInfo = StringUtils::streamToString(output);
        os_->closeOpenedStream(output);

        ScoringResultBuilder scoringResult;
        scoringResult.setExecutionResult(executionResult);

        if (privateInfo.empty()) {
            scoringResult.setVerdict(Verdict::ac());
        } else {
            scoringResult
                    .setVerdict(Verdict::wa())
                    .setPrivateInfo(privateInfo);
        }

        return scoringResult.build();
    }
};

}

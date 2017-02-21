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
    static constexpr const char* DIFF_FILENAME = "_diff.out";

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
                .setOutputFilename(DIFF_FILENAME)
                .build());
        istream* output = os_->openForReading(DIFF_FILENAME);
        string message = StringUtils::streamToString(output);
        os_->closeOpenedStream(output);

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

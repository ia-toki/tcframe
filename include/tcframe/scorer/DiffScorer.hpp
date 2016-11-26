#pragma once

#include <streambuf>
#include <string>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/os.hpp"
#include "tcframe/verdict.hpp"

using std::istreambuf_iterator;
using std::string;

namespace tcframe {

class DiffScorer : public Scorer {
private:
    OperatingSystem* os_;

public:
    virtual ~DiffScorer() {}

    DiffScorer(OperatingSystem* os)
            : os_(os) {}

    ScoringResult score(
            const string&,
            const string& outputFilename,
            const string& evaluationFilename) {

        string briefDiffCommand = "diff --brief " + outputFilename + " " + evaluationFilename;
        ExecutionResult briefResult = os_->execute(ExecutionRequestBuilder().setCommand(briefDiffCommand).build());

        if (briefResult.info().isSuccessful()) {
            return ScoringResultBuilder()
                    .setVerdict(Verdict::ac())
                    .build();
        }

        string diffCommand = string() +
                 "diff " +
                 "--unchanged-line-format=' %.2dn    %L' " +
                 "--old-line-format='(expected) [line %.2dn]    %L' " +
                 "--new-line-format='(received) [line %.2dn]    %L' " +
                 outputFilename + " " + evaluationFilename;
        string scoringCommand = diffCommand + " | head -n 10";
        ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename("_diff.out")
                .build());

        string diff = string(istreambuf_iterator<char>(*result.outputStream()), istreambuf_iterator<char>());

        return ScoringResultBuilder()
                .setVerdict(Verdict::wa())
                .setMessage("Diff:\n" + diff)
                .build();
    }
};

}

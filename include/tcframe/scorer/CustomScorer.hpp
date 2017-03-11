#pragma once

#include <stdexcept>
#include <streambuf>
#include <string>

#include "Scorer.hpp"
#include "ScoringResultParser.hpp"
#include "tcframe/grade.hpp"
#include "tcframe/os.hpp"

using std::string;

namespace tcframe {

class CustomScorer : public Scorer {
private:
    static constexpr const char* SCORING_FILENAME = "_scoring.out";

    OperatingSystem* os_;
    string scorerCommand_;

public:
    CustomScorer(OperatingSystem* os, const string& scorerCommand)
            : os_(os)
            , scorerCommand_(scorerCommand) {}

    ScoringResult score(const string& inputFilename, const string& outputFilename, const string& evaluationFilename) {
        string scoringCommand = scorerCommand_
                + " " + inputFilename
                + " " + outputFilename
                + " " + evaluationFilename;

        ExecutionResult executionResult = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename(SCORING_FILENAME)
                .build());


        if (executionResult.isSuccessful()) {
            istream* output = os_->openForReading(SCORING_FILENAME);
            ScoringResult result = ScoringResultParser::parse(output);
            os_->closeOpenedStream(output);
            return result;
        } else {
            return ScoringResultBuilder()
                    .setExecutionResult(executionResult)
                    .setVerdict(Verdict::err())
                    .build();
        }
    }
};

}

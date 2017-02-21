#pragma once

#include <stdexcept>
#include <streambuf>
#include <string>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/os.hpp"
#include "tcframe/verdict.hpp"

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

        ExecutionResult result = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename(SCORING_FILENAME)
                .build());
        Verdict verdict;
        string message;
        istream* output = os_->openForReading(SCORING_FILENAME);
        try {
            verdict = result.isSuccessful()
                    ? VerdictParser::parse(output)
                    : Verdict::err();
        } catch (runtime_error& e) {
            verdict = Verdict::err();
            message = e.what();
        }
        os_->closeOpenedStream(output);

        return ScoringResultBuilder()
                .setExecutionResult(result)
                .setVerdict(verdict)
                .setMessage(message)
                .build();
    }
};

}

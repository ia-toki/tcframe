#pragma once

#include <istream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <utility>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"

using std::istream;
using std::move;
using std::runtime_error;
using std::string;

namespace tcframe {

class CustomScorer : public Scorer {
private:
    OperatingSystem* os_;
    VerdictCreator* verdictCreator_;
    string scorerCommand_;

public:
    virtual ~CustomScorer() = default;

    CustomScorer(OperatingSystem* os, VerdictCreator* verdictCreator, string scorerCommand)
            : os_(os)
            , verdictCreator_(verdictCreator)
            , scorerCommand_(move(scorerCommand)) {}

    ScoringResult score(const string& inputFilename, const string& outputFilename, const string& evaluationFilename) {
        string scoringCommand = scorerCommand_
                + " " + inputFilename
                + " " + outputFilename
                + " " + evaluationFilename;

        ExecutionResult executionResult = os_->execute(ExecutionRequestBuilder()
                .setCommand(scoringCommand)
                .setOutputFilename(SCORING_OUT_FILENAME)
                .build());

        Verdict verdict;
        if (executionResult.isSuccessful()) {
            istream* output = os_->openForReading(SCORING_OUT_FILENAME);
            try {
                verdict = verdictCreator_->fromStream(output);
            } catch (runtime_error& e) {
                verdict = Verdict(VerdictStatus::err());

                string newStandardError = executionResult.standardError()
                        + "\n"
                        + e.what();
                executionResult = ExecutionResultBuilder(executionResult)
                        .setStandardError(newStandardError)
                        .build();
            }
            os_->closeOpenedStream(output);
        } else {
            verdict = Verdict(VerdictStatus::err());
        }

        return {verdict, executionResult};
    }
};

}

#pragma once

#include <stdexcept>
#include <string>
#include <sys/signal.h>
#include <utility>

#include "../EvaluationOptions.hpp"
#include "CommunicationResult.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::runtime_error;
using std::string;

namespace tcframe {

class Communicator {
private:
    static constexpr const char* COMMUNICATION_PIPE_FILENAME = "__tcframe_communication.pipe";

    OperatingSystem* os_;
    VerdictCreator* verdictCreator_;
    string communicatorCommand_;

public:
    virtual ~Communicator() = default;

    Communicator(OperatingSystem* os, VerdictCreator* verdictCreator, string communicatorCommand)
            : os_(os)
            , verdictCreator_(verdictCreator)
            , communicatorCommand_(move(communicatorCommand)) {}

    virtual CommunicationResult communicate(const string& inputFilename, const EvaluationOptions& options) {
        string communicationCommand = string()
                + "rm -f " + COMMUNICATION_PIPE_FILENAME + " && "
                + "mkfifo " + COMMUNICATION_PIPE_FILENAME + " && "
                + communicatorCommand_ + " " + inputFilename + " < " + COMMUNICATION_PIPE_FILENAME
                + " | "
                + options.solutionCommand() + " > " + COMMUNICATION_PIPE_FILENAME;

        auto request = ExecutionRequestBuilder()
                .setCommand(communicationCommand);

        if (options.timeLimit()) {
            request.setTimeLimit(options.timeLimit().value());
        }
        if (options.memoryLimit()) {
            request.setMemoryLimit(options.memoryLimit().value());
        }

        ExecutionResult executionResult = ignoreSIGPIPE(os_->execute(request.build()));

        Verdict verdict;
        optional<Verdict> maybeVerdict = verdictCreator_->fromExecutionResult(executionResult);

        if (maybeVerdict) {
            verdict = maybeVerdict.value();
        } else {
            istream* output = new istringstream(executionResult.standardError());
            try {
                verdict = verdictCreator_->fromStream(output);
                executionResult = ignoreStderr(executionResult);
            } catch (runtime_error& e) {
                verdict = Verdict(VerdictStatus::err());
                executionResult = ExecutionResultBuilder(executionResult)
                        .setStandardError(e.what())
                        .build();
            }
        }

        return {verdict, executionResult};
    }

private:
    static ExecutionResult ignoreSIGPIPE(const ExecutionResult& executionResult) {
        if (executionResult.exitSignal() == optional<int>(SIGPIPE)) {
            return ExecutionResultBuilder(executionResult)
                    .setExitCode(0)
                    .build();
        }
        return executionResult;
    }

    static ExecutionResult ignoreStderr(const ExecutionResult& executionResult) {
        return ExecutionResultBuilder(executionResult)
                .setStandardError("")
                .build();
    }
};

}

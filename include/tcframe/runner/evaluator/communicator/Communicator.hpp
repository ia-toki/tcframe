#pragma once

#include <stdexcept>
#include <string>
#include <sys/signal.h>

#include "../EvaluationOptions.hpp"
#include "CommunicationResult.hpp"
#include "tcframe/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

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
    virtual ~Communicator() {}

    Communicator(OperatingSystem* os, VerdictCreator* verdictCreator, const string& communicatorCommand)
            : os_(os)
            , verdictCreator_(verdictCreator)
            , communicatorCommand_(communicatorCommand) {}

    virtual CommunicationResult communicate(const string& inputFilename, const EvaluationOptions& options) {
        string communicationCommand = string()
                + "rm -f " + COMMUNICATION_PIPE_FILENAME + " && "
                + "mkfifo " + COMMUNICATION_PIPE_FILENAME + " && "
                + communicatorCommand_ + " " + inputFilename + " < " + COMMUNICATION_PIPE_FILENAME
                + " | "
                + options.solutionCommand() + " > " + COMMUNICATION_PIPE_FILENAME;

        ExecutionRequestBuilder request = ExecutionRequestBuilder()
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
                executionResult = ExecutionResultBuilder()
                        .from(executionResult)
                        .setStandardError(e.what())
                        .build();
            }
        }

        return CommunicationResult(verdict, executionResult);
    }

private:
    static ExecutionResult ignoreSIGPIPE(const ExecutionResult& executionResult) {
        if (executionResult.exitSignal() == optional<int>(SIGPIPE)) {
            return ExecutionResultBuilder()
                    .from(executionResult)
                    .setExitCode(0)
                    .build();
        }
        return executionResult;
    }

    static ExecutionResult ignoreStderr(const ExecutionResult& executionResult) {
        return ExecutionResultBuilder()
                .from(executionResult)
                .setStandardError("")
                .build();
    }
};

}

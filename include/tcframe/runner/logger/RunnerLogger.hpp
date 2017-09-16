#pragma once

#include <string>
#include <vector>

#include "LoggerEngine.hpp"

using std::string;
using std::vector;

namespace tcframe {

class RunnerLogger {
private:
    LoggerEngine* engine_;

public:
    virtual ~RunnerLogger() = default;

    explicit RunnerLogger(LoggerEngine* engine)
            : engine_(engine) {}

    virtual void logSpecificationFailure(const vector<string>& messages) {
        engine_->logHeading("SPECIFICATIONS");
        engine_->logParagraph(1, "The specifications contain errors:");
        for (const string& message : messages) {
            engine_->logListItem1(1, message);
        }
    }
};

class RunnerLoggerFactory {
public:
    virtual ~RunnerLoggerFactory() = default;

    virtual RunnerLogger* create(LoggerEngine* engine) {
        return new RunnerLogger(engine);
    }
};

}

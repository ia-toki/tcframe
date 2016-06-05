#pragma once

#include "SpecificationFailure.hpp"
#include "tcframe/logger.hpp"

namespace tcframe {

class RunnerLogger : public BaseLogger {
public:
    RunnerLogger(LoggerEngine* engine)
            : BaseLogger(engine)
    {}

    virtual void logSpecificationFailure(SpecificationFailure failure) {
        engine_->logHeading("SPECIFICATIONS");
        engine_->logParagraph(1, "The specifications contain errors:");
        for (const string& message : failure.messages()) {
            engine_->logListItem1(1, message);
        }
    }
};

}

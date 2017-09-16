#pragma once

#include "BriefGraderLogger.hpp"
#include "DefaultGraderLogger.hpp"
#include "GraderLogger.hpp"
#include "tcframe/runner/logger.hpp"

namespace tcframe {

class GraderLoggerFactory {
public:
    virtual ~GraderLoggerFactory() = default;

    virtual GraderLogger* create(LoggerEngine* engine, bool brief) {
        if (brief) {
            return new BriefGraderLogger(engine);
        }
        return new DefaultGraderLogger(engine);
    }
};

}

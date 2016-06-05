#pragma once

#include "RunnerLogger.hpp"
#include "tcframe/logger.hpp"

namespace tcframe {

class RunnerLoggerFactory {
public:
    virtual ~RunnerLoggerFactory() {}

    virtual RunnerLogger* create(LoggerEngine* engine) {
        return new RunnerLogger(engine);
    }
};

}

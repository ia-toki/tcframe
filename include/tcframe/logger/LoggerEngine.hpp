#pragma once

#include <string>

using std::string;

namespace tcframe {

class LoggerEngine {
public:
    virtual ~LoggerEngine() {}

    virtual void logHeading(string message) = 0;
    virtual void logParagraph(int level, string message) = 0;
    virtual void logHangingParagraph(int level, string message) = 0;
    virtual void logListItem1(int level, string message) = 0;
    virtual void logListItem2(int level, string message) = 0;
};

}

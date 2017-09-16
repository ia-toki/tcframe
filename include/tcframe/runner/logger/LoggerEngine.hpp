#pragma once

#include <string>

using std::string;

namespace tcframe {

class LoggerEngine {
public:
    virtual ~LoggerEngine() = default;

    virtual void logHeading(const string& message) = 0;
    virtual void logParagraph(int level, const string& message) = 0;
    virtual void logHangingParagraph(int level, const string& message) = 0;
    virtual void logListItem1(int level, const string& message) = 0;
    virtual void logListItem2(int level, const string& message) = 0;
};

}

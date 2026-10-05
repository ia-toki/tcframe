#pragma once

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "GraderLogger.hpp"
#include "tcframe/runner/logger.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::string;
using std::vector;

namespace tcframe {

// Machine-readable grading result for `grade --format=json` (SPEC.md T3.4).
// Printed once, from logResult(), as a single line:
//   {"verdict":{"code":"AC","points":100.00},
//    "subtasks":[{"id":1,"verdict":{"code":"AC","points":100.00}}],
//    "testcases":[{"name":"aplusb_1_1","verdict":"AC","points":null}]}
// "points" is null when a test case verdict carries no points.
class JsonGraderLogger : public GraderLogger {
private:
    LoggerEngine* engine_;
    string currentTestCaseName_;
    vector<string> testCases_;

public:
    virtual ~JsonGraderLogger() = default;

    explicit JsonGraderLogger(LoggerEngine* engine)
            : engine_(engine) {}

    void logTestGroupIntroduction(int) {}
    void logTestCaseIntroduction(const string& name) {
        currentTestCaseName_ = name;
    }
    void logError(runtime_error*) {}
    void logIntroduction(const string&) {}

    void logTestCaseVerdict(const TestCaseVerdict& verdict) {
        string points = verdict.points() ? number(verdict.points().value()) : "null";
        testCases_.push_back("{\"name\":" + quote(currentTestCaseName_)
                + ",\"verdict\":" + quote(verdict.verdict().code())
                + ",\"points\":" + points + "}");
    }

    void logResult(const map<int, SubtaskVerdict>& subtaskVerdicts, const SubtaskVerdict& verdict) {
        string subtasks;
        for (const auto& entry : subtaskVerdicts) {
            if (!subtasks.empty()) {
                subtasks += ",";
            }
            subtasks += "{\"id\":" + StringUtils::toString(entry.first)
                    + ",\"verdict\":" + verdictJson(entry.second) + "}";
        }

        string testCases;
        for (const string& testCase : testCases_) {
            if (!testCases.empty()) {
                testCases += ",";
            }
            testCases += testCase;
        }

        engine_->logParagraph(0, "{\"verdict\":" + verdictJson(verdict)
                + ",\"subtasks\":[" + subtasks + "]"
                + ",\"testcases\":[" + testCases + "]}");
    }

private:
    static string verdictJson(const SubtaskVerdict& verdict) {
        return "{\"code\":" + quote(verdict.verdict().code())
                + ",\"points\":" + number(verdict.points()) + "}";
    }

    static string number(double value) {
        return StringUtils::toString(value, 2);
    }

    static string quote(const string& text) {
        string out = "\"";
        for (char c : text) {
            switch (c) {
                case '"':
                    out += "\\\"";
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case '\n':
                    out += "\\n";
                    break;
                case '\r':
                    out += "\\r";
                    break;
                case '\t':
                    out += "\\t";
                    break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        char buffer[8];
                        snprintf(buffer, sizeof(buffer), "\\u%04x", static_cast<unsigned char>(c));
                        out += buffer;
                    } else {
                        out += c;
                    }
            }
        }
        return out + "\"";
    }
};

}

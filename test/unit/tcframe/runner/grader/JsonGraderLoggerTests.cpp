#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../logger/MockLoggerEngine.hpp"
#include "tcframe/runner/grader/JsonGraderLogger.hpp"

using ::testing::Test;

namespace tcframe {

class JsonGraderLoggerTests : public Test {
protected:
    MOCK(LoggerEngine) engine;

    JsonGraderLogger logger = JsonGraderLogger(&engine);
};

TEST_F(JsonGraderLoggerTests, Result_SingleSubtask) {
    SubtaskVerdict verdict(Verdict::ac(), 100);
    EXPECT_CALL(engine, logParagraph(0,
            "{\"verdict\":{\"code\":\"AC\",\"points\":100.00},"
            "\"subtasks\":[{\"id\":1,\"verdict\":{\"code\":\"AC\",\"points\":100.00}}],"
            "\"testcases\":[]}"));
    logger.logResult({{1, verdict}}, verdict);
}

TEST_F(JsonGraderLoggerTests, Result_TestCasesAndSubtasks) {
    logger.logTestGroupIntroduction(1);
    logger.logTestCaseIntroduction("a\"b");
    logger.logTestCaseVerdict(TestCaseVerdict(Verdict::wa(), 0));
    logger.logTestCaseIntroduction("c");
    logger.logTestCaseVerdict(TestCaseVerdict(Verdict::ac()));

    SubtaskVerdict verdict(Verdict::wa(), 25.5);
    SubtaskVerdict subtask1(Verdict::ac(), 25.5);
    SubtaskVerdict subtask2(Verdict::wa(), 0);
    EXPECT_CALL(engine, logParagraph(0,
            "{\"verdict\":{\"code\":\"WA\",\"points\":25.50},"
            "\"subtasks\":["
            "{\"id\":1,\"verdict\":{\"code\":\"AC\",\"points\":25.50}},"
            "{\"id\":2,\"verdict\":{\"code\":\"WA\",\"points\":0.00}}],"
            "\"testcases\":["
            "{\"name\":\"a\\\"b\",\"verdict\":\"WA\",\"points\":0.00},"
            "{\"name\":\"c\",\"verdict\":\"AC\",\"points\":null}]}"));
    logger.logResult({{1, subtask1}, {2, subtask2}}, verdict);
}

TEST_F(JsonGraderLoggerTests, Name_EscapesControlCharacters) {
    logger.logTestCaseIntroduction(std::string("x\\y\n\x01"));
    logger.logTestCaseVerdict(TestCaseVerdict(Verdict::rte()));

    SubtaskVerdict verdict(Verdict::rte(), 0);
    EXPECT_CALL(engine, logParagraph(0,
            "{\"verdict\":{\"code\":\"RTE\",\"points\":0.00},"
            "\"subtasks\":[{\"id\":1,\"verdict\":{\"code\":\"RTE\",\"points\":0.00}}],"
            "\"testcases\":[{\"name\":\"x\\\\y\\n\\u0001\",\"verdict\":\"RTE\",\"points\":null}]}"));
    logger.logResult({{1, verdict}}, verdict);
}

}

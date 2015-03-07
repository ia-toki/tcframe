#include "gtest/gtest.h"

#include "tcframe/tcframe.hpp"

using tcframe::Constraint;
using tcframe::ConstraintsCollector;
using tcframe::GridIOSegment;
using tcframe::IOFormat;
using tcframe::IOFormatsCollector;
using tcframe::IOMode;
using tcframe::IOSegment;
using tcframe::LineIOSegment;
using tcframe::LinesIOSegment;
using tcframe::MatrixSizes;
using tcframe::Subtask;
using tcframe::TestCase;
using tcframe::TestCasesCollector;
using tcframe::TestGroup;
using tcframe::VectorSize;

TEST(MacroTest, SIZE_IMPL1) {
    VectorSize vs = SIZE(2);

    EXPECT_EQ(2, *vs.getSize());
}

TEST(MacroTest, SIZE_IMPL2) {
    MatrixSizes ms = SIZE(2, 3);

    EXPECT_EQ(2, *ms.getRowsSize());
    EXPECT_EQ(3, *ms.getColumnsize());
}

class ConstraintsCollectorTester : public ConstraintsCollector {
public:
    ConstraintsCollectorTester()
            : A(42) { }

    void test() {
        CONS(A == 7);
        CONS(A == 42);
    }

private:
    int A;
};

TEST(MacroTest, CONS) {
    ConstraintsCollectorTester tester;
    tester.test();

    vector<Subtask*> subtasks = tester.collectSubtasks();

    ASSERT_EQ(1, subtasks.size());

    vector<Constraint*> constraints = subtasks[0]->getConstraints();

    ASSERT_EQ(2, constraints.size());
    EXPECT_EQ("A == 7", constraints[0]->getDescription());
    EXPECT_EQ("A == 42", constraints[1]->getDescription());
}

class TestCasesCollectorTester : public TestCasesCollector {
public:
    void test() {
        CASE(A = 7);
        CASE(A = 42);
    }

private:
    int A;
};

TEST(MacroTest, CASE) {
    TestCasesCollectorTester tester;
    tester.test();

    vector<TestGroup*> testGroups = tester.collectTestData();

    ASSERT_EQ(2, testGroups.size());

    ASSERT_EQ(2, testGroups[1]->getTestCasesCount());
    EXPECT_EQ("(A = 7)", testGroups[1]->getTestCase(0)->getDescription());
    EXPECT_EQ("(A = 42)", testGroups[1]->getTestCase(1)->getDescription());
}

class IOFormatsCollectorTester : public IOFormatsCollector {
public:
    void test() {
        LINE(A, W % SIZE(4));
        LINES(V) % SIZE(5);
        EMPTY_LINE();
        GRID(G) % SIZE(2, 3);
    }

private:
    int A, B, C;
    vector<int> V, W;
    vector<vector<int>> G;
};

TEST(MacroTest, IOFormats) {
    IOFormatsCollectorTester tester;
    tester.test();

    IOFormat* format = tester.collectFormat(IOMode::INPUT);
    vector<IOSegment*> segments = format->getSegments();

    ASSERT_EQ(4, segments.size());

    EXPECT_NE(nullptr, dynamic_cast<LineIOSegment*>(segments[0]));
    EXPECT_NE(nullptr, dynamic_cast<LinesIOSegment*>(segments[1]));
    EXPECT_NE(nullptr, dynamic_cast<LineIOSegment*>(segments[2]));
    EXPECT_NE(nullptr, dynamic_cast<GridIOSegment*>(segments[3]));
}
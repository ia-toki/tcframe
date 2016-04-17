#include "gtest/gtest.h"

#include "tcframe/deprecated/ioformat/IOFormatProvider.hpp"
#include "tcframe/deprecated/ioformat/MatrixSizes.hpp"
#include "tcframe/deprecated/ioformat/VectorSize.hpp"
#include "tcframe/deprecated/generator/TestCasesCollector.hpp"
#include "tcframe/deprecated/generator/TestGroup.hpp"
#include "tcframe/deprecated/problem/Constraint.hpp"
#include "tcframe/deprecated/problem/ConstraintsCollector.hpp"
#include "tcframe/deprecated/problem/Subtask.hpp"
#include "tcframe/deprecated/runner/Macro.hpp"

#include <vector>

using std::vector;

using tcframe_old::Constraint;
using tcframe_old::ConstraintsCollector;
using tcframe_old::IOFormatProvider;
using tcframe_old::MatrixSizes;
using tcframe_old::Subtask;
using tcframe_old::TestCasesCollector;
using tcframe_old::TestGroup;
using tcframe_old::VectorSize;

TEST(DeprecatedMacroTest, SIZE_IMPL1) {
    VectorSize vs = SIZE(2);

    EXPECT_EQ(2, vs.getSize());
}

TEST(DeprecatedMacroTest, SIZE_IMPL2) {
    MatrixSizes ms = SIZE(2, 3);

    EXPECT_EQ(2, ms.getRowsSize());
    EXPECT_EQ(3, ms.getColumnsize());
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

TEST(DeprecatedMacroTest, CONS) {
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
        SAMPLE_CASE({
            "123"
        });

        CASE(A = 7);
        CASE(A = 42);
    }

private:
    int A;
};

TEST(DeprecatedMacroTest, CASE) {
    TestCasesCollectorTester tester;
    tester.test();

    vector<TestGroup*> testGroups = tester.collectTestData();

    ASSERT_EQ(2, testGroups.size());

    ASSERT_EQ(1, testGroups[0]->getTestCasesCount());
    EXPECT_EQ("", testGroups[0]->getTestCase(0)->getDescription());

    ASSERT_EQ(2, testGroups[1]->getTestCasesCount());
    EXPECT_EQ("A = 7", testGroups[1]->getTestCase(0)->getDescription());
    EXPECT_EQ("A = 42", testGroups[1]->getTestCase(1)->getDescription());
}

class IOFormatProviderTester : public IOFormatProvider {
public:
    IOFormatProviderTester() {
        this->A = 1;
        this->B = 2;
        this->V = vector<int>{1, 2, 3, 4, 5};
        this->W = vector<int>{6, 7, 8, 9};
        this->Y = vector<int>{100, 200};
        this->G = vector<vector<int>>{ {0, 0, 0}, {1, 1, 1}};
    }

    void test() {
        LINE(A, W % SIZE(4), Y);
        LINES(V) % SIZE(5);
        EMPTY_LINE();
        GRID(G) % SIZE(2, 3);
    }

private:
    int A, B, C;
    vector<int> V, W, Y;
    vector<vector<int>> G;
};

TEST(DeprecatedMacroTest, IOSegments) {
    IOFormatProviderTester tester;

    ostringstream sout;

    tester.beginPrintingFormat(&sout);
    tester.test();
    tester.endPrintingFormat();

    EXPECT_EQ("1 6 7 8 9 100 200\n1\n2\n3\n4\n5\n\n0 0 0\n1 1 1\n", sout.str());
}

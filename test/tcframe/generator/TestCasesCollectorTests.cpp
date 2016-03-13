#include "gtest/gtest.h"

#include "tcframe/generator/TestCasesCollector.hpp"
#include "tcframe/generator/TestGroup.hpp"

#include <vector>

using std::vector;

using tcframe::TestCasesCollector;
using tcframe::TestGroup;

TEST(TestCasesCollectorTest, WithTestGroups) {
    TestCasesCollector collector;

    int X;

    collector.newTestGroup();
    collector.addOfficialTestCase([&] { X = 1; }, "X = 1");

    collector.newTestGroup();
    collector.addOfficialTestCase([&] { X = 2; }, "X = 2");
    collector.addOfficialTestCase([&] { X = 3; }, "X = 3");

    vector<TestGroup*> testGroups = collector.collectTestData();

    ASSERT_EQ(3, testGroups.size());

    EXPECT_EQ(0, testGroups[0]->getId());
    EXPECT_EQ(0, testGroups[0]->getTestCasesCount());

    EXPECT_EQ(1, testGroups[1]->getId());
    EXPECT_EQ(1, testGroups[1]->getTestCasesCount());

    EXPECT_EQ(2, testGroups[2]->getId());
    EXPECT_EQ(2, testGroups[2]->getTestCasesCount());
}

TEST(TestCasesCollectorTest, WithoutTestGroups) {
    TestCasesCollector collector;

    int X;

    collector.addOfficialTestCase([&] { X = 1; }, "X = 1");
    collector.addOfficialTestCase([&] { X = 2; }, "X = 2");
    collector.addOfficialTestCase([&] { X = 3; }, "X = 3");

    vector<TestGroup*> testGroups = collector.collectTestData();

    ASSERT_EQ(2, testGroups.size());

    EXPECT_EQ(0, testGroups[0]->getId());
    EXPECT_EQ(0, testGroups[0]->getTestCasesCount());

    EXPECT_EQ(-1, testGroups[1]->getId());
    EXPECT_EQ(3, testGroups[1]->getTestCasesCount());
}

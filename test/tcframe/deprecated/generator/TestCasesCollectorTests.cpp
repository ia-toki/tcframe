#include "gtest/gtest.h"

#include "tcframe/deprecated/generator/TestCasesCollector.hpp"
#include "tcframe/deprecated/generator/TestGroup.hpp"

#include <vector>

using std::vector;

using tcframe_old::TestCasesCollector;
using tcframe_old::TestGroup;

TEST(DeprecatedTestCasesCollectorTest, WithTestGroups) {
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

TEST(DeprecatedTestCasesCollectorTest, WithoutTestGroups) {
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

#ifndef CPTEST_GENERATOR_H
#define CPTEST_GENERATOR_H

#include "exception.h"
#include "os.h"
#include "problem.h"
#include "testcase.h"

#include <vector>
#include <cstdio>
#include <iostream>

using std::cout;
using std::endl;
using std::initializer_list;
using std::vector;

namespace org { namespace iatoki { namespace cptest {

template<typename TProblem>
class BaseGenerator : protected TProblem {
private:
    OperatingSystem* os;
    TestCasesCollector* testCasesCollector;

    vector<void(BaseGenerator::*)()> testGroupBlocks = {
        &BaseGenerator::TestGroup1,
        &BaseGenerator::TestGroup2,
        &BaseGenerator::TestGroup3,
        &BaseGenerator::TestGroup4,
        &BaseGenerator::TestGroup5
    };

    vector<TestGroup*> collectTestData() {
        try {
            TestCases();
            return testCasesCollector->collectTestData();
        } catch (NotImplementedException e1){
            for (auto testGroupBlock : testGroupBlocks) {
                try {
                    testCasesCollector->newTestGroup();
                    (this->*testGroupBlock)();
                } catch (NotImplementedException e2) {
                    vector<TestGroup*> testCases = testCasesCollector->collectTestData();
                    testCases.pop_back();
                    return testCases;
                }
            }
        }
    }

protected:
    BaseGenerator() :
        os(new Unix()),
        testCasesCollector(new TestCasesCollector())
    { }

    BaseGenerator(OperatingSystem* os) :
        os(os),
        testCasesCollector(new TestCasesCollector())
    { }

    virtual ~BaseGenerator() { }

    virtual void TestCases() { throw NotImplementedException(); }

    virtual void TestGroup1() { throw NotImplementedException(); }
    virtual void TestGroup2() { throw NotImplementedException(); }
    virtual void TestGroup3() { throw NotImplementedException(); }
    virtual void TestGroup4() { throw NotImplementedException(); }
    virtual void TestGroup5() { throw NotImplementedException(); }


    void addTestCase(function<void()> closure, string description) {
        testCasesCollector->addTestCase(closure, description);
    }

    void assignToSubtasks(initializer_list<int> subtaskNos) {
        testCasesCollector->assignToSubtasks(subtaskNos);
    }

public:
    void generate() {
        TProblem::setCurrentFormatAsInputFormat();
        TProblem::InputFormat();

        vector<Subtask*> subtasks = TProblem::collectSubtasks();
        vector<TestGroup*> testData = collectTestData();
        
        for (TestGroup* testGroup : testData) {
            if (testGroup->id) {
                cout << "[Test Group " << testGroup->id << "]" << endl;
            }

            for (TestCase* testCase : testGroup->testCases) {
                cout << "Test Case " << testCase->id << endl;
                TProblem::printTestCaseInput(testCase, cout);
            }
        }
    }
};

}}}

#endif
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
class BaseGenerator : protected TProblem, protected TestCasesCollector {
private:
    OperatingSystem* os;

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
            return TestCasesCollector::collectTestData();
        } catch (NotImplementedException e1){
            for (auto testGroupBlock : testGroupBlocks) {
                try {
                    TestCasesCollector::newTestGroup();
                    (this->*testGroupBlock)();
                } catch (NotImplementedException e2) {
                    vector<TestGroup*> testCases = TestCasesCollector::collectTestData();
                    testCases.pop_back();
                    return testCases;
                }
            }
        }
    }

protected:
    BaseGenerator() : os(new Unix()) { }

    BaseGenerator(OperatingSystem* os) : os(os) { }

    virtual ~BaseGenerator() { }

    virtual void TestCases() { throw NotImplementedException(); }

    virtual void TestGroup1() { throw NotImplementedException(); }
    virtual void TestGroup2() { throw NotImplementedException(); }
    virtual void TestGroup3() { throw NotImplementedException(); }
    virtual void TestGroup4() { throw NotImplementedException(); }
    virtual void TestGroup5() { throw NotImplementedException(); }

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
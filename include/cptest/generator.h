#ifndef CPTEST_GENERATOR_H
#define CPTEST_GENERATOR_H

#include "exception.h"
#include "problem.h"
#include "testdata.h"

#include <vector>
#include <cstdio>
#include <iostream>

using std::cout;
using std::initializer_list;
using std::vector;

namespace org { namespace iatoki { namespace cptest {

template<typename TProblem>
class BaseGenerator : protected TProblem {
private:
    vector<TestSet> sampleTestData;
    vector<TestSet> testData;

    TestSet currentTestSet;

    vector<void(BaseGenerator::*)()> testSetBlocks = {
        &BaseGenerator::testSet1,
        &BaseGenerator::testSet2,
        &BaseGenerator::testSet3,
        &BaseGenerator::testSet4,
        &BaseGenerator::testSet5
    };


protected:
    void addSampleTestCase(TestCase testCase, initializer_list<int> subtasksNumbers) {
        TestSet testSet;
        
        testSet.addTestCase(testCase);
        for (int subtaskNumber : subtasksNumbers) {
            testSet.assignToSubtask(subtaskNumber);
        }

        sampleTestData.push_back(testSet);
    }

    void addTestCase(TestCase testCase) {
        currentTestSet.addTestCase(testCase);
    }

    void assignToSubtasks(initializer_list<int> subtasksNumbers) {
        for (int subtaskNumber : subtasksNumbers) {
            currentTestSet.assignToSubtask(subtaskNumber);
        }
    }

public:
    virtual void testSet1() { throw NotImplementedException(); }
    virtual void testSet2() { throw NotImplementedException(); }
    virtual void testSet3() { throw NotImplementedException(); }
    virtual void testSet4() { throw NotImplementedException(); }
    virtual void testSet5() { throw NotImplementedException(); }

    void generate() {
        TProblem::setCurrentFormatAsInputFormat();
        TProblem::inputFormat();
        TProblem::collectSubtasks();

        for (auto testSetBlock : testSetBlocks) {
            try {
                (this->*testSetBlock)();
                testData.push_back(currentTestSet);
                currentTestSet = TestSet();
            } catch (NotImplementedException e) {
                break;
            }
        }

        int i = 0;
        for (auto testSet : testData) {
            printf("Test set %d\n", i++);

            int j = 0;
            for (auto testCase : testSet.getTestCases()) {
                printf("    Test case %d\n", j++);

                testCase.execute();
                TProblem::printInputTo(cout);

                for (int subtaskNumber : testSet.getSubtaskNumbers()) {
                    for (auto predicate : TProblem::getFailedPredicates(subtaskNumber)) {
                        printf("    failed on predicate %s\n", predicate.getDescription().c_str());
                    }
                }
            }
        }
    }
};

}}}

#endif
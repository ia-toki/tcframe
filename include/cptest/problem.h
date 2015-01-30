#ifndef CPTEST_PROBLEM_H
#define CPTEST_PROBLEM_H

#include "constraint.h"
#include "exception.h"
#include "io.h"
#include "testcase.h"

#include <vector>

using std::vector;

namespace org { namespace iatoki { namespace cptest {

class BaseProblem : protected ConstraintsCollector, protected IOFormatsCollector {
private:
    string slug;

    vector<void(BaseProblem::*)()> subtaskBlocks = {
        &BaseProblem::Subtask1,
        &BaseProblem::Subtask2,
        &BaseProblem::Subtask3,
        &BaseProblem::Subtask4,
        &BaseProblem::Subtask5
    };

protected:
    BaseProblem() : slug("problem") { }

    virtual ~BaseProblem() { }

    virtual void Config() = 0;
    virtual void InputFormat() = 0;
    virtual void Constraints() { throw NotImplementedException(); }
    virtual void Subtask1() { throw NotImplementedException(); }
    virtual void Subtask2() { throw NotImplementedException(); }
    virtual void Subtask3() { throw NotImplementedException(); }
    virtual void Subtask4() { throw NotImplementedException(); }
    virtual void Subtask5() { throw NotImplementedException(); }

    void setSlug(string slug) {
        this->slug = slug;
    }

    string getSlug() {
        return slug;
    }

    vector<Subtask*> collectSubtasks() {
        try {
            Constraints();
            return ConstraintsCollector::collectSubtasks();
        } catch (NotImplementedException e1){
            for (auto subtaskBlock : subtaskBlocks) {
                try {
                    ConstraintsCollector::newSubtask();
                    (this->*subtaskBlock)();
                } catch (NotImplementedException e2) {
                    vector<Subtask*> subtasks = ConstraintsCollector::collectSubtasks();
                    subtasks.pop_back();
                    return subtasks;
                }
            }
        }
    }

    void printTestCaseInput(TestCase* testCase, ostream& out) {
        testCase->closure();
        IOFormatsCollector::printFormatTo(IOMode::INPUT, out);
    }
};

}}}

#endif
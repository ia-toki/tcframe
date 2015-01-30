#ifndef CPTEST_PROBLEM_H
#define CPTEST_PROBLEM_H

#include "constraint.h"
#include "exception.h"
#include "io.h"
#include "testcase.h"

#include <vector>

using std::vector;

namespace org { namespace iatoki { namespace cptest {

class BaseProblem {
private:
    string slug;
    IOFormat* inputFormat;
    IOFormat* curFormat;

    ConstraintsCollector* constraintsCollector;

    vector<void(BaseProblem::*)()> subtaskBlocks = {
        &BaseProblem::Subtask1,
        &BaseProblem::Subtask2,
        &BaseProblem::Subtask3,
        &BaseProblem::Subtask4,
        &BaseProblem::Subtask5
    };

protected:
    BaseProblem() :
        slug("problem"),
        inputFormat(new IOFormat()),
        curFormat(nullptr),
        constraintsCollector(new ConstraintsCollector())
    { }

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

    void addConstraint(function<bool()> predicate, string description) {
        constraintsCollector->addConstraint(predicate, description);
    }

    void setCurrentFormatAsInputFormat() {
        curFormat = this->inputFormat;
    }

    LineIOSegment& line() {
        LineIOSegment* segment = new LineIOSegment();
        curFormat->addSegment(segment);
        return *segment;
    }

    vector<Subtask*> collectSubtasks() {
        try {
            Constraints();
            return constraintsCollector->collectSubtasks();
        } catch (NotImplementedException e1){
            for (auto subtaskBlock : subtaskBlocks) {
                try {
                    constraintsCollector->newSubtask();
                    (this->*subtaskBlock)();
                } catch (NotImplementedException e2) {
                    vector<Subtask*> subtasks = constraintsCollector->collectSubtasks();
                    subtasks.pop_back();
                    return subtasks;
                }
            }
        }
    }

    void printTestCaseInput(TestCase* testCase, ostream& out) {
        testCase->closure();
        inputFormat->printTo(out);
    }
};

}}}

#endif
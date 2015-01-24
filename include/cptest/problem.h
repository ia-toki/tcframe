#ifndef CPTEST_PROBLEM_H
#define CPTEST_PROBLEM_H

#include "constraint.h"
#include "exception.h"
#include "io.h"

#include <vector>

using std::vector;

namespace org { namespace iatoki { namespace cptest {

class BaseProblem {
private:
    vector<Subtask> subtasks;

    IOFormat* inFormat;
    IOFormat* outFormat;

    IOFormat* curFormat;

    Subtask currentSubtask;

    vector<void(BaseProblem::*)()> subtaskBlocks = {
        &BaseProblem::subtask1,
        &BaseProblem::subtask2,
        &BaseProblem::subtask3,
        &BaseProblem::subtask4,
        &BaseProblem::subtask5
    };

protected:

    BaseProblem() {
        inFormat = new IOFormat();
        outFormat = new IOFormat();
        curFormat = nullptr;
    }

    virtual void subtask1() { throw NotImplementedException(); }
    virtual void subtask2() { throw NotImplementedException(); }
    virtual void subtask3() { throw NotImplementedException(); }
    virtual void subtask4() { throw NotImplementedException(); }
    virtual void subtask5() { throw NotImplementedException(); }

    void addPredicate(Predicate pred) {
        currentSubtask.addPredicate(pred);
    }

    void setCurrentFormatAsInputFormat() {
        curFormat = inFormat;
    }

    LineIOSegment& line() {
        LineIOSegment* segment = new LineIOSegment();
        curFormat->addSegment(segment);
        return *segment;
    }

    void collectSubtasks() {
        for (auto subtaskBlock : subtaskBlocks) {
            try {
                (this->*subtaskBlock)();
                subtasks.push_back(currentSubtask);
                currentSubtask = Subtask();
            } catch (NotImplementedException e) {
                break;
            }
        }
    }

    vector<Predicate> getFailedPredicates(int subtaskNumber) {
        return subtasks[subtaskNumber].getFailedPredicates();
    }

public: 
    virtual void inputFormat() = 0;

    void printInputTo(ostream& out) {
        inFormat->printTo(out);
    }

};

}}}

#endif
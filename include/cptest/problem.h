#ifndef CPTEST_PROBLEM_H
#define CPTEST_PROBLEM_H

#include "constraint.h"
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
protected:

    BaseProblem() {
        inFormat = new IOFormat();
        outFormat = new IOFormat();
        curFormat = nullptr;
    }

    void addPredicate(Predicate pred) {
        subtasks.back().addPredicate(pred);
    }

    void setCurrentFormatAsInputFormat() {
        curFormat = inFormat;
    }

    LineIOSegment& line() {
        LineIOSegment* segment = new LineIOSegment();
        curFormat->addSegment(segment);
        return *segment;
    }

public: 
    virtual void inputFormat() = 0;

    void printInputTo(ostream& out) {
        inFormat->printTo(out);
    }

};

}}}

#endif
#ifndef TCFRAME_PROBLEM_H
#define TCFRAME_PROBLEM_H

#include "constraint.hpp"
#include "exception.hpp"
#include "io.hpp"
#include "testcase.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class BaseProblem : protected ConstraintsCollector, protected IOFormatsCollector {
protected:
    BaseProblem()
            : slug("problem") { }

    virtual ~BaseProblem() { }

    virtual void Config() { }

    void setSlug(string slug) {
        this->slug = slug;
    }

    string getSlug() {
        return slug;
    }

    virtual void InputFormat() = 0;

    virtual void Constraints() { throw NotImplementedException(); }
    virtual void Subtask1() { throw NotImplementedException(); }
    virtual void Subtask2() { throw NotImplementedException(); }
    virtual void Subtask3() { throw NotImplementedException(); }
    virtual void Subtask4() { throw NotImplementedException(); }
    virtual void Subtask5() { throw NotImplementedException(); }

    vector<Subtask*> getSubtasks() {
        try {
            Constraints();
            return ConstraintsCollector::collectSubtasks();
        } catch (NotImplementedException& e1) {
            for (auto subtaskBlock : subtaskBlocks) {
                try {
                    ConstraintsCollector::newSubtask();
                    (this->*subtaskBlock)();
                } catch (NotImplementedException& e2) {
                    vector<Subtask*> subtasks = ConstraintsCollector::collectSubtasks();
                    subtasks.pop_back();
                    return subtasks;
                }
            }

            return ConstraintsCollector::collectSubtasks();
        }
    }

    IOFormat* getInputFormat() {
        InputFormat();
        return IOFormatsCollector::collectFormat(IOMode::INPUT);
    }

private:
    string slug;

    vector<void(BaseProblem::*)()> subtaskBlocks = {
            &BaseProblem::Subtask1,
            &BaseProblem::Subtask2,
            &BaseProblem::Subtask3,
            &BaseProblem::Subtask4,
            &BaseProblem::Subtask5
    };
};

}

#endif

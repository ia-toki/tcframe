#ifndef TCFRAME_PROBLEM_H
#define TCFRAME_PROBLEM_H

#include "constraint.hpp"
#include "exception.hpp"
#include "io.hpp"
#include "testcase.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class BaseProblem : protected ConstraintsCollector, protected IOFormatProvider {
public:
    void applyConfiguration() {
        Config();
    }

    void setSlug(string slug) {
        this->slug = slug;
    }

    string getSlug() {
        return slug;
    }

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
protected:
    BaseProblem()
            : slug("problem") { }

    virtual ~BaseProblem() { }

    virtual void Config() { }

    virtual void InputFormat() = 0;
    virtual void OutputFormat() = 0;

    virtual void Constraints() { throw NotImplementedException(); }
    virtual void Subtask1() { throw NotImplementedException(); }
    virtual void Subtask2() { throw NotImplementedException(); }
    virtual void Subtask3() { throw NotImplementedException(); }
    virtual void Subtask4() { throw NotImplementedException(); }
    virtual void Subtask5() { throw NotImplementedException(); }
    virtual void Subtask6() { throw NotImplementedException(); }
    virtual void Subtask7() { throw NotImplementedException(); }
    virtual void Subtask8() { throw NotImplementedException(); }
    virtual void Subtask9() { throw NotImplementedException(); }
    virtual void Subtask10() { throw NotImplementedException(); }

private:
    string slug;

    vector<void(BaseProblem::*)()> subtaskBlocks = {
            &BaseProblem::Subtask1,
            &BaseProblem::Subtask2,
            &BaseProblem::Subtask3,
            &BaseProblem::Subtask4,
            &BaseProblem::Subtask5,
            &BaseProblem::Subtask6,
            &BaseProblem::Subtask7,
            &BaseProblem::Subtask8,
            &BaseProblem::Subtask9,
            &BaseProblem::Subtask10,
    };
};

}

#endif

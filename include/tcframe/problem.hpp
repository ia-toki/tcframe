#ifndef TCFRAME_PROBLEM_H
#define TCFRAME_PROBLEM_H

#include "constraint.hpp"
#include "exception.hpp"
#include "io.hpp"
#include "testcase.hpp"

#include <getopt.h>
#include <vector>

using std::vector;

namespace tcframe {

class BaseProblem : protected ConstraintsCollector, protected IOFormatProvider {
public:
    void applyProblemConfiguration() {
        Config();
    }

    void applyProblemCommandLineOptions(int argc, char* argv[]) {
        option longopts[6] = {
                { "slug", required_argument, nullptr, 's'},
                { "time-limit", required_argument, nullptr, 't'},
                { "no-time-limit", no_argument, nullptr, 'T'},
                { "memory-limit", required_argument, nullptr, 'm'},
                { "no-memory-limit", no_argument, nullptr, 'M'},
                { 0, 0, 0, 0 }
        };

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 's':
                    slug = string(optarg);
                    break;
                case 't':
                    timeLimit = Util::toInt(string(optarg));
                    break;
                case 'T':
                    timeLimit = 0;
                    break;
                case 'm':
                    memoryLimit = Util::toInt(string(optarg));
                    break;
                case 'M':
                    memoryLimit = 0;
                    break;
                default:
                    break;
            }
        }
    }

    string getSlug() {
        return slug;
    }

    int getTimeLimit() {
        return timeLimit;
    }

    int getMemoryLimit() {
        return memoryLimit;
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
            : slug("problem"), timeLimit(0), memoryLimit(0) { }

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

    void setSlug(string slug) {
        this->slug = slug;
    }

    void setTimeLimit(int timeLimitInSeconds) {
        this->timeLimit = timeLimitInSeconds;
    }

    void setMemoryLimit(int memoryLimitInMegabytes) {
        this->memoryLimit = memoryLimitInMegabytes;
    }

private:
    string slug;
    int timeLimit;
    int memoryLimit;

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

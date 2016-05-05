#pragma once

#include "tcframe/deprecated/core/NotImplementedException.hpp"
#include "tcframe/deprecated/ioformat/IOFormatProvider.hpp"
#include "tcframe/deprecated/problem/Constraint.hpp"
#include "tcframe/deprecated/problem/ConstraintsCollector.hpp"
#include "tcframe/deprecated/problem/Subtask.hpp"

#include <getopt.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe_old {

class BaseProblem : protected ConstraintsCollector, protected IOFormatProvider {
public:
    virtual void applyProblemConfiguration() {
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

        optind = 1;
        opterr = 0;

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

    bool isMultipleTestCasesPerFile() {
        return multipleTestCasesCount != nullptr;
    }

    int* getMultipleTestCasesCountPointer() {
        return multipleTestCasesCount;
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

    vector<Constraint*> getMultipleTestCasesConstraints() {
        ConstraintsCollector::setMultipleTestCasesMode();
        MultipleTestCasesConstraints();
        return ConstraintsCollector::collectMultipleTestCasesConstraints();
    }

protected:
    virtual ~BaseProblem() { }

    virtual void Config() { }

    virtual void InputFormat() = 0;
    virtual void OutputFormat() { throw NotImplementedException(); }

    virtual void MultipleTestCasesConstraints() { }

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
    virtual void Subtask11() { throw NotImplementedException(); }
    virtual void Subtask12() { throw NotImplementedException(); }
    virtual void Subtask13() { throw NotImplementedException(); }
    virtual void Subtask14() { throw NotImplementedException(); }
    virtual void Subtask15() { throw NotImplementedException(); }
    virtual void Subtask16() { throw NotImplementedException(); }
    virtual void Subtask17() { throw NotImplementedException(); }
    virtual void Subtask18() { throw NotImplementedException(); }
    virtual void Subtask19() { throw NotImplementedException(); }
    virtual void Subtask20() { throw NotImplementedException(); }
    virtual void Subtask21() { throw NotImplementedException(); }
    virtual void Subtask22() { throw NotImplementedException(); }
    virtual void Subtask23() { throw NotImplementedException(); }
    virtual void Subtask24() { throw NotImplementedException(); }
    virtual void Subtask25() { throw NotImplementedException(); }

    void setSlug(string slug) {
        this->slug = slug;
    }

    void setTimeLimit(int timeLimitInSeconds) {
        this->timeLimit = timeLimitInSeconds;
    }

    void setMemoryLimit(int memoryLimitInMegabytes) {
        this->memoryLimit = memoryLimitInMegabytes;
    }

    void setMultipleTestCasesCount(int& testCasesCount) {
        this->multipleTestCasesCount = &testCasesCount;
    }

private:
    string slug = "problem";
    int timeLimit = 0;
    int memoryLimit = 0;
    int* multipleTestCasesCount = nullptr;

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
            &BaseProblem::Subtask11,
            &BaseProblem::Subtask12,
            &BaseProblem::Subtask13,
            &BaseProblem::Subtask14,
            &BaseProblem::Subtask15,
            &BaseProblem::Subtask16,
            &BaseProblem::Subtask17,
            &BaseProblem::Subtask18,
            &BaseProblem::Subtask19,
            &BaseProblem::Subtask20,
            &BaseProblem::Subtask21,
            &BaseProblem::Subtask22,
            &BaseProblem::Subtask23,
            &BaseProblem::Subtask24,
            &BaseProblem::Subtask25
    };
};

}

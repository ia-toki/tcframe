#ifndef TCFRAME_RUNNER_H
#define TCFRAME_RUNNER_H

#include "generator.hpp"
#include "macro.hpp"
#include "submitter.hpp"

#include <getopt.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace tcframe {

enum RunnerMode {
    GENERATION,
    SUBMISSION
};

template<typename TProblem>
class Runner {
public:
    Runner(int argc, char** argv)
            : argc(argc), argv(argv), problem(new TProblem()), mode(RunnerMode::GENERATION) { }

    Runner(int argc, char** argv, TProblem* problem)
            : argc(argc), argv(argv), problem(problem), mode(RunnerMode::GENERATION) { }

    void setGenerator(BaseGenerator<TProblem>* generator) {
        this->generator = generator;
    }

    void applyCommandLineOptions() {
        option longopts[5] = {
                { "slug", required_argument, nullptr, 's'},
                { "tc-dir", required_argument, nullptr, 'd'},
                { "solution-command", required_argument, nullptr, 'c'},
                { "porcelain", no_argument, &isPorcelain, 1},
                { 0, 0, 0, 0 }
        };

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 's':
                    problem->setSlug(string(optarg));
                    break;
                case 'd':
                    generator->setTestCasesDir(string(optarg));
                    break;
                case 'c':
                    generator->setSolutionCommand(string(optarg));
                    break;
                case '?':
                case ':':
                    exit(1);
                default:
                    break;
            }
        }
    }

    int run() {
        if (argc > 1 && string(argv[1]) == "submit") {
            mode = RunnerMode::SUBMISSION;

            if (argc == 2) {
                submissionCommand = generator->getSolutionCommand();
                argc--;
                argv++;
            } else {
                submissionCommand = string(argv[2]);
                argc -= 2;
                argv += 2;
            }
        }

        problem->applyConfiguration();
        generator->applyConfiguration();

        applyCommandLineOptions();

        if (mode == RunnerMode::GENERATION) {
            return generator->generate();
        } else {
            Submitter<TProblem>* submitter = new Submitter<TProblem>(problem, generator);
            submitter->setPorcelain((bool) isPorcelain);
            return submitter->submit(submissionCommand);
        }
    }

private:
    int argc;
    char** argv;

    RunnerMode mode;
    string submissionCommand;

    int isPorcelain;

    TProblem* problem;
    BaseGenerator<TProblem>* generator;
};

}

#endif

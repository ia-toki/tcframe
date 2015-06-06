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
            : argc(argc), argv(argv), mode(RunnerMode::GENERATION), isPorcelain(0) { }

    void setGenerator(BaseGenerator<TProblem>* generator) {
        this->generator = generator;
    }

    void applyCommandLineOptions() {
        option longopts[9] = {
                { "slug", required_argument, nullptr, 's'},
                { "tc-dir", required_argument, nullptr, 'd'},
                { "time-limit", required_argument, nullptr, 't'},
                { "no-time-limit", no_argument, nullptr, 'T'},
                { "memory-limit", required_argument, nullptr, 'm'},
                { "no-memory-limit", no_argument, nullptr, 'M'},
                { "solution-command", required_argument, nullptr, 'c'},
                { "porcelain", no_argument, &isPorcelain, 1},
                { 0, 0, 0, 0 }
        };

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 's':
                    generator->setSlug(string(optarg));
                    break;
                case 'd':
                    generator->setTestCasesDir(string(optarg));
                    break;
                case 'c':
                    generator->setSolutionCommand(string(optarg));
                    break;
                case 't':
                    generator->setTimeLimit(Util::toInt(string(optarg)));
                    break;
                case 'T':
                    generator->setTimeLimit(0);
                    break;
                case 'm':
                    generator->setMemoryLimit(Util::toInt(string(optarg)));
                    break;
                case 'M':
                    generator->setMemoryLimit(0);
                    break;
                case '?':
                case ':':
                    exit(1);
                default:
                    break;
            }
        }

        args.clear();
        for (int i = optind; i < argc; i++) {
            args.push_back(string(argv[i]));
        }
    }

    int run() {
        string program = argv[0];

        if (argc > 1) {
            if (string(argv[1]) == "submit") {
                mode = RunnerMode::SUBMISSION;
                argc--;
                argv++;
            } else if (argv[1][0] != '-') {
                cout << "Unknown runner command: " << string(argv[1]) << endl;
                return 1;
            }
        }

        generator->applyConfiguration();

        applyCommandLineOptions();

        if (mode == RunnerMode::GENERATION) {
            return generator->generate();
        } else {
            if (args.size() > 1) {
                cout << "Usage: " << program << " submit [ <submissionCommand> ]" << endl;
                return 1;
            }

            string submissionCommand;
            if (args.empty()) {
                submissionCommand = generator->getSolutionCommand();
            } else {
                submissionCommand = args[0];
            }

            Submitter<TProblem>* submitter = new Submitter<TProblem>(generator);
            submitter->setPorcelain((bool) isPorcelain);
            return submitter->submit(submissionCommand);
        }
    }

private:
    int argc;
    char** argv;
    vector<string> args;

    RunnerMode mode;

    int isPorcelain;

    BaseGenerator<TProblem>* generator;
};

}

#endif

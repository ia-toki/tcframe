#pragma once

#include "tcframe/deprecated/generator/BaseGenerator.hpp"
#include "tcframe/deprecated/submitter/Submitter.hpp"

#include <getopt.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace tcframe_old {

enum RunnerMode {
    GENERATION,
    SUBMISSION
};

template<typename TProblem>
class Runner {
public:
    Runner(int argc, char** argv)
            : argc(argc), argv(argv), mode(RunnerMode::GENERATION) { }

    void setGenerator(BaseGenerator<TProblem>* generator) {
        this->generator = generator;
    }

    int run() {
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

        generator->applyProblemConfiguration();
        generator->applyProblemCommandLineOptions(argc, argv);


        if (mode == RunnerMode::GENERATION) {
            generator->applyGeneratorConfiguration();
            generator->applyGeneratorCommandLineOptions(argc, argv);

            return generator->generate();
        } else {
            generator->applyGeneratorConfiguration();
            generator->applyGeneratorCommandLineOptions(argc, argv);

            Submitter<TProblem>* submitter = new Submitter<TProblem>(generator);
            string result = submitter->applySubmitterCommandLineOptions(argc, argv);

            if (result != "") {
                cout << result << endl;
                return 1;
            }

            return submitter->submit();
        }
    }

private:
    int argc;
    char** argv;

    RunnerMode mode;

    BaseGenerator<TProblem>* generator;
};

}

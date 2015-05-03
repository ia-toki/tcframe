#ifndef TCFRAME_RUNNER_H
#define TCFRAME_RUNNER_H

#include "generator.hpp"
#include "macro.hpp"

#include <getopt.h>
#include <string>

using std::string;

namespace tcframe {

template<typename TProblem>
class Runner {
public:
    Runner(int argc, char** argv)
            : argc(argc), argv(argv) { }

    void setGenerator(BaseGenerator<TProblem>* generator) {
        this->generator = generator;
    }

    void applyCommandLineOptions() {
        option longopts[4] = {
                { "slug", required_argument, nullptr, 's'},
                { "tc-dir", required_argument, nullptr, 'd'},
                { "solution-command", required_argument, nullptr, 'c'},
                { 0, 0, 0, 0 }
        };

        int c;
        while ((c = getopt_long(argc, argv, "", longopts, nullptr)) != -1) {
            switch (c) {
                case 's':
                    generator->setProblemSlug(string(optarg));
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
        generator->applyConfigurations();

        applyCommandLineOptions();

        return generator->generate();
    }

private:
    int argc;
    char** argv;

    BaseGenerator<TProblem>* generator;
};

}

#endif

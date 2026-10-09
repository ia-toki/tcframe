#pragma once

#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <stdexcept>
#include <string>

#include "Args.hpp"
#include "tcframe/util.hpp"

using std::runtime_error;
using std::string;

namespace tcframe {

class ArgsParser {
public:
    ArgsParser() = delete;

    static Args parse(int argc, char* argv[]) {
        option longopts[] = {
                { "allow-unsatisfied-subtasks", no_argument      , nullptr, 'a'},
                { "brief",                      no_argument      , nullptr, 'b'},
                { "communicator",               required_argument, nullptr, 'c'},
                { "memory-limit",               required_argument, nullptr, 'd'},
                { "no-memory-limit",            no_argument      , nullptr, 'e'},
                { "no-time-limit",              no_argument      , nullptr, 'f'},
                { "output",                     required_argument, nullptr, 'g'},
                { "scorer",                     required_argument, nullptr, 'h'},
                { "seed",                       required_argument, nullptr, 'i'},
                { "solution",                   required_argument, nullptr, 'j'},
                { "time-limit",                 required_argument, nullptr, 'k'},
                { 0, 0, 0, 0 }};

        Args args;
        args.command_ = Args::Command::GENERATE;
        if (argc >= 2 && strcmp(argv[1], "grade") == 0) {
            args.command_ = Args::Command::GRADE;
            argc--;
            argv++;
        }

        optind = 1;
        opterr = 0;

        int c;
        while ((c = getopt_long_only(argc, argv, ":", longopts, nullptr)) != -1) {
            switch (c) {
                case 'a':
                    args.allowUnsatisfiedSubtasks_ = true;
                    break;
                case 'b':
                    args.brief_ = true;
                    break;
                case 'c':
                    args.communicator_ = optional<string>(optarg);
                    break;
                case 'd':
                    args.memoryLimit_ = StringUtils::toNumber<int>(optarg);
                    break;
                case 'e':
                    args.noMemoryLimit_ = true;
                    break;
                case 'f':
                    args.noTimeLimit_ = true;
                    break;
                case 'g':
                    args.output_ = optional<string>(optarg);
                    break;
                case 'h':
                    args.scorer_ = optional<string>(optarg);
                    break;
                case 'i':
                    args.seed_ = StringUtils::toNumber<unsigned>(optarg);
                    break;
                case 'j':
                    args.solution_ = optional<string>(optarg);
                    break;
                case 'k':
                    args.timeLimit_ = StringUtils::toNumber<int>(optarg);
                    break;
                case ':':
                    throw runtime_error("tcframe: option " + string(argv[optind - 1]) + " requires an argument");
                case '?':
                    throw runtime_error("tcframe: option " + string(argv[optind - 1]) + " unknown");
                default:
                    break;
            }
        }

        return args;
    }
};

}

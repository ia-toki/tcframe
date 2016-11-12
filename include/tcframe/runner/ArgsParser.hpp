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
    static Args parse(int argc, char* argv[]) {
        option longopts[] = {
                { "memory-limit",    required_argument, nullptr, 'a'},
                { "no-memory-limit", no_argument      , nullptr, 'b'},
                { "no-time-limit",   no_argument      , nullptr, 'c'},
                { "seed",            required_argument, nullptr, 'd'},
                { "solution",        required_argument, nullptr, 'e'},
                { "output",          required_argument, nullptr, 'f'},
                { "time-limit",      required_argument, nullptr, 'g'},
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
                    args.memoryLimit_ = StringUtils::toNumber<int>(optarg);
                    break;
                case 'b':
                    args.noMemoryLimit_ = true;
                    break;
                case 'c':
                    args.noTimeLimit_ = true;
                    break;
                case 'd':
                    args.seed_ = StringUtils::toNumber<unsigned>(optarg);
                    break;
                case 'e':
                    args.solution_ = optional<string>(optarg);
                    break;
                case 'f':
                    args.output_ = optional<string>(optarg);
                    break;
                case 'g':
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

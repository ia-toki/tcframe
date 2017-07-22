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
                { "brief",           no_argument,       nullptr, 'a'},
                { "communicator",    required_argument, nullptr, 'b'},
                { "memory-limit",    required_argument, nullptr, 'c'},
                { "no-memory-limit", no_argument      , nullptr, 'd'},
                { "no-time-limit",   no_argument      , nullptr, 'e'},
                { "output",          required_argument, nullptr, 'f'},
                { "scorer",          required_argument, nullptr, 'g'},
                { "seed",            required_argument, nullptr, 'h'},
                { "solution",        required_argument, nullptr, 'i'},
                { "time-limit",      required_argument, nullptr, 'j'},
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
                    args.brief_ = true;
                    break;
                case 'b':
                    args.communicator_ = optional<string>(optarg);
                    break;
                case 'c':
                    args.memoryLimit_ = StringUtils::toNumber<int>(optarg);
                    break;
                case 'd':
                    args.noMemoryLimit_ = true;
                    break;
                case 'e':
                    args.noTimeLimit_ = true;
                    break;
                case 'f':
                    args.output_ = optional<string>(optarg);
                    break;
                case 'g':
                    args.scorer_ = optional<string>(optarg);
                    break;
                case 'h':
                    args.seed_ = StringUtils::toNumber<unsigned>(optarg);
                    break;
                case 'i':
                    args.solution_ = optional<string>(optarg);
                    break;
                case 'j':
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

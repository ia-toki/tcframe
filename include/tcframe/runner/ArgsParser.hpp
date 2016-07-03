#pragma once

#include <cstdio>
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
                { "seed",       required_argument, nullptr, 'a'},
                { "slug",       required_argument, nullptr, 'b'},
                { "solution",   required_argument, nullptr, 'c'},
                { "tc-dir",     required_argument, nullptr, 'd'},
                { 0, 0, 0, 0 }};

        Args args;
        args.command_ = Args::Command::GEN;

        optind = 1;
        opterr = 0;

        int c;
        unsigned seed;
        while ((c = getopt_long_only(argc, argv, ":", longopts, nullptr)) != -1) {
            switch (c) {
                case 'a':
                    sscanf(optarg, "%u", &seed);
                    args.seed_ = optional<unsigned>(seed);
                    break;
                case 'b':
                    args.slug_ = optional<string>(optarg);
                    break;
                case 'c':
                    args.solution_ = optional<string>(optarg);
                    break;
                case 'd':
                    args.tcDir_ = optional<string>(optarg);
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

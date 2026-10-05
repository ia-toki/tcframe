#pragma once

#include <map>
#include <string>
#include <vector>

#include "tcframe/util.hpp"

using std::map;
using std::string;
using std::vector;

namespace tcframe {

// Aggregator declared for a subtask. Empty slug = not declared (1.x implicit selection).
struct AggregatorYaml {
    string slug;
    string args;
};

struct SubtaskYaml {
    double points;
    AggregatorYaml aggregator;
};

// Helper program/config declared in spec.yml. Empty slug = use registry default.
struct HelperYaml {
    string slug;
    string additional_args;
};

struct EvaluatorYaml {
    string slug;
    vector<string> solution_keys = {"source"};
    bool tc_output_present;
    bool has_scorer;
};

struct LimitsYaml {
    int time_s;
    int memory_mb;
};

struct SpecYaml {
    string slug;
    vector<SubtaskYaml> subtasks;
    EvaluatorYaml evaluator;
    map<string, HelperYaml> helpers;
    LimitsYaml limits;
};

}

#pragma once

#include <string>
#include <vector>

#include "tcframe/util.hpp"

using std::string;
using std::vector;

namespace tcframe {

struct SubtaskYaml {
    double points;
};

struct EvaluatorYaml {
    string slug;
    bool has_tc_output;
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
    LimitsYaml limits;
};

}

#pragma once

#include <stdexcept>
#include <string>

#include "MinAggregator.hpp"
#include "SubtaskAggregator.hpp"
#include "SumAggregator.hpp"
#include "TestCaseAggregator.hpp"
#include "ThresholdAggregator.hpp"

using std::invalid_argument;
using std::runtime_error;
using std::string;

namespace tcframe {

class AggregatorRegistry {
public:
    virtual ~AggregatorRegistry() = default;

    // slug: declared aggregator of a subtask ("min", "sum", "threshold"); empty = not declared,
    // in which case the 1.x implicit choice applies (min when the spec has subtasks, else sum).
    // args: slug-specific argument, e.g. the threshold for "threshold".
    virtual TestCaseAggregator* getTestCaseAggregator(const string& slug, const string& args, bool hasSubtasks) {
        if (slug.empty()) {
            if (hasSubtasks) {
                return new MinAggregator();
            }
            return new SumAggregator();
        }
        if (slug == "min") {
            return new MinAggregator();
        }
        if (slug == "sum") {
            return new SumAggregator();
        }
        if (slug == "threshold") {
            return new ThresholdAggregator(parseThreshold(args));
        }
        throw runtime_error("unknown aggregator: " + slug);
    }

    virtual SubtaskAggregator* getSubtaskAggregator() {
        return new SubtaskAggregator();
    }

private:
    static double parseThreshold(const string& args) {
        try {
            size_t end = 0;
            double threshold = std::stod(args, &end);
            if (end == args.size()) {
                return threshold;
            }
        } catch (const std::logic_error&) {
            // fall through to the error below
        }
        throw invalid_argument("threshold aggregator needs a numeric threshold, got '" + args + "'");
    }
};

}

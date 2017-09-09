#include "tcframe/runner.hpp"

#include __TCFRAME_SPEC_FILE__

using tcframe::Driver;
using tcframe::Runner;

int main(int argc, char* argv[]) {
    Driver<ProblemSpec> driver(
            __TCFRAME_SPEC_FILE__,
            new TestSpec());

    Runner<ProblemSpec> runner(
            &driver,
            new SimpleLoggerEngine(),
            new OperatingSystem(),
            new RunnerLoggerFactory(),
            new GraderLoggerFactory(),
            new GeneratorFactory(),
            new GraderFactory(),
            new EvaluatorRegistry(new EvaluatorHelperRegistry()),
            new AggregatorRegistry());

    return runner.run(argc, argv);
}

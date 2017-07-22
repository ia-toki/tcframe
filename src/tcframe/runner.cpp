#include "tcframe/runner/main.hpp"

#include __TCFRAME_SPEC_FILE__

using tcframe::RunnerMain;

int main(int argc, char* argv[]) {
    RunnerMain<ProblemSpec> runner(
            __TCFRAME_SPEC_FILE__,
            new TestSpec(),
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

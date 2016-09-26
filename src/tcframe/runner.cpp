#include "tcframe/generator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/experimental/runner.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/submitter.hpp"

#include __TCFRAME_SPEC_FILE__

using tcframe::Runner;

int main(int argc, char* argv[]) {
    auto testSpec = new TestSpec();
    auto loggerEngine = new SimpleLoggerEngine();
    auto os = new UnixOperatingSystem();
    auto runnerLoggerFactory = new RunnerLoggerFactory();
    auto generatorFactory = new GeneratorFactory();
    auto submitterFactory = new SubmitterFactory();

    Runner<ProblemSpec> runner(
            testSpec,
            loggerEngine,
            os,
            runnerLoggerFactory,
            generatorFactory,
            submitterFactory);

    return runner.run(argc, argv);
}

#include "tcframe/runner.hpp"
#include "tcframe/spec.hpp"

#include __TCFRAME_SPEC_FILE__

using tcframe::Runner;

int main(int argc, char* argv[]) {
    Runner<ProblemSpec> runner(new TestSpec());
    return runner.run(argc, argv);
}

#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    vector<int> A;

    int positive_count;
    vector<string> answers;

    void InputFormat() {
        LINE(N);
        LINE(A % SIZE(N));
    }

    void OutputFormat() {
        LINES(answers) % SIZE(positive_count);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(64);
    }

    void Constraints() {
        CONS(1 <= N && N <= 10);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void BeforeTestCase() {
        A.clear();
    }

    void AfterTestCase() {
        positive_count = count_if(A.begin(), A.end(), [](int a) {return a > 0;});
    }

    void SampleTestCase1() {
        Input({
            "5",
            "4 -1 3 2 -5",
        });
        Output({
            "even",
            "odd",
            "even"
        });
    }

    void TestCases() {
        CASE(N = 6, A = {-1, 2, 3, -4, -5, 6});
    }
};

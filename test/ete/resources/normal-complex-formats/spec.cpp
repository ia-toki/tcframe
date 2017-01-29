#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    string S;
    int N;
    vector<int> A;
    vector<int> X, Y;
    vector<vector<int>> M;

    string res;
    vector<string> answers;

    void InputFormat() {
        RAW_LINE(S);
        LINE(N);
        LINE(A % SIZE(2));
        LINES(X, Y) % SIZE(N - 1);
        GRID(M) % SIZE(2, 3);
    }

    void OutputFormat() {
        LINE(res);
        RAW_LINES(answers);
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
    void SampleTestCase1() {
        Input({
            "[BEGIN INPUT]",
            "3",
            "3 5",
            "1 1",
            "2 2",
            "7 7 7",
            "8 8 8"
        });
        Output({
            "yes",
            "lorem ipsum",
            " dolor sit amet! "
        });
    }

    void TestCases() {
        CASE(S = "[BEGIN INPUT]", N = 3, A = {10, 20}, X = {0, 0}, Y = {1, 1}, M = { {1, 2, 3}, {4, 5, 6} });
    }
};

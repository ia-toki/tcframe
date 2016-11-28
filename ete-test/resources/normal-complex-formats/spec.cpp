#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    vector<int> A;
    vector<int> X, Y;
    vector<vector<int>> M;

    string res;

    void InputFormat() {
        LINE(N);
        LINE(A % SIZE(2));
        LINES(X, Y) % SIZE(N);
        GRID(M) % SIZE(2, 3);
    }

    void OutputFormat() {
        LINE(res);
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
            "2",
            "3 5",
            "1 1",
            "2 2",
            "7 7 7",
            "8 8 8"
        });
        Output({
            "yes"
        });
    }

    void TestCases() {
        CASE(N = 2, A = {10, 20}, X = {0, 0}, Y = {1, 1}, M = { {1, 2, 3}, {4, 5, 6} });
    }
};

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

    vector<string> res_impossible;
    string res;
    vector<string> answers;

    void InputFormat() {
        RAW_LINE(S);
        LINE(N);
        LINE(A % SIZE(2));
        LINES(X, Y) % SIZE(N - 1);
        GRID(M) % SIZE(2, 3);
    }

    void OutputFormat1() {
        LINE(res_impossible % SIZE(3));
    }

    void OutputFormat2() {
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

    void SampleTestCase2() {
        Input({
            "[BEGIN INPUT]",
            "1",
            "3 5",
            "7 7 7",
            "8 8 8"
        });
        Output({
            "no it's impossible"
        });
    }

    void TestCases() {
        CASE(S = "[BEGIN INPUT]", N = 1, A = {10, 20}, X = {}, Y = {}, M = { {1, 2, 3}, {4, 5, 6} });
        CASE(S = "[BEGIN INPUT]", N = 3, A = {10, 20}, X = {0, 0}, Y = {1, 1}, M = { {1, 2, 3}, {4, 5, 6} });
    }
};

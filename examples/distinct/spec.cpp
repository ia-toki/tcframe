// Distinct Count — migrated from tcframe-python/examples/distinct.
//
// Problem:
//   Given N integers, count the number of distinct values.
//
// Input:
//   N
//   A_1 A_2 ... A_N
//
// Output:
//   K   (number of distinct values)
//
// Subtask 1 (30 pts): 1 <= N <= 1000,   1 <= A_i <= 1000
// Subtask 2 (70 pts): 1 <= N <= 200000, 1 <= A_i <= 1e9
//
// Build and generate test cases (from examples/distinct):
//   /path/to/tcframe-v2/scripts/tcframe make --solution=solutions/ref/solution.cpp
//
// Check every solutions/ entry against its expected verdict:
//   /path/to/tcframe-v2/scripts/tcframe test

#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    vector<int> A;
    int K;  // answer

    void InputFormat() {
        LINE(N);
        LINE(A);
    }

    void OutputFormat() {
        LINE(K);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(256);
    }

    void Constraints() {
        CONS(valueOf(N).isBetween(1, 200000));
    }

    void Subtask1() {
        Points(30);

        CONS(valueOf(N).isBetween(1, 1000));
        CONS(eachElementOf(A).isBetween(1, 1000));
    }

    void Subtask2() {
        Points(70);

        CONS(valueOf(N).isBetween(1, 200000));
        CONS(eachElementOf(A).isBetween(1, 1000000000));
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1, 2});
        Input({
            "5",
            "3 1 4 1 5"
        });
        Output({
            "4"
        });
    }

    void SampleTestCase2() {
        Subtasks({1, 2});
        Input({
            "4",
            "7 7 7 7"
        });
        Output({
            "1"
        });
    }

    void TestGroup1() {
        Subtasks({1, 2});

        CASE(N = 1, A = {1}, K = 1);
        CASE(N = 5, A = {1, 1, 1, 1, 1}, K = 1);
        CASE(N = 5, A = {1, 2, 3, 4, 5}, K = 5);

        for (int i = 0; i < 7; i++) {
            int n = rnd.nextInt(1, 1000);
            vector<int> a(n);
            for (int& x : a) x = rnd.nextInt(1, 1000);
            set<int> distinct(a.begin(), a.end());
            CASE(N = n, A = a, K = (int) distinct.size());
        }
    }

    void TestGroup2() {
        Subtasks({2});

        // max N, all same
        CASE(N = 200000, A = vector<int>(200000, 42), K = 1);

        // max N, all distinct
        vector<int> vals(200000);
        iota(vals.begin(), vals.end(), 1);
        CASE(N = 200000, A = vals, K = 200000);

        for (int i = 0; i < 8; i++) {
            int n = rnd.nextInt(50000, 200000);
            vector<int> a(n);
            for (int& x : a) x = rnd.nextInt(1, 1000000000);
            set<int> distinct(a.begin(), a.end());
            CASE(N = n, A = a, K = (int) distinct.size());
        }
    }
};

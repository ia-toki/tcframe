#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    ifstream tc_in(argv[1]);
    ifstream tc_out(argv[2]);
    ifstream con_out(argv[3]);

    int A, B;
    tc_in >> A >> B;

    double tc_answer;
    tc_out >> tc_answer;

    double con_answer;
    con_out >> con_answer;

    if (B == 4) {
        return 1 / (B - 4);
    } else if (abs(tc_answer - con_answer) < 1e-1) {
        cout << "AC" << endl;
    } else {
        cout << "WA" << endl;
    }
}

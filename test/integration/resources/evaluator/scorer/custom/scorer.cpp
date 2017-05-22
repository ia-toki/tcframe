#include <bits/stdc++.h>
using namespace std;

/*
 * AC if contestant's second line is a permutation of judge's.
 * OK 50 if not, but the largest elements are the same.
 * */

int N;

vector<int> read(ifstream& out) {
    string answer;
    out >> answer;

    if (answer == "crash") {
        // division by zero
        return vector<int>(1 / (N - 3));
    }
    if (answer == "bogus") {
        // bogus output
        cout << "bogus" << endl;
        exit(0);
    }

    vector<int> numbers(N);
    for (int i = 0; i < N; i++) {
        out >> numbers[i];
    }
    sort(numbers.begin(), numbers.end());
    return numbers;
}

int main(int argc, char* argv[]) {
    ifstream tc_in(argv[1]);
    ifstream tc_out(argv[2]);
    ifstream con_out(argv[3]);

    tc_in >> N;

    vector<int> ans_judge = read(tc_out);
    vector<int> ans_con = read(con_out);

    if (ans_judge == ans_con) {
        cout << "AC" << endl;
    } else if (ans_judge.back() == ans_con.back()) {
        cout << "OK" << endl;
        cout << "50" << endl;
    } else {
        cout << "WA" << endl;
    }
}

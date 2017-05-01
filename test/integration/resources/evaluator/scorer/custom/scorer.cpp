#include <bits/stdc++.h>
using namespace std;

/* AC if contestant's second line is a permutation of judge's. */

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

    if (read(tc_out) == read(con_out)) {
        cout << "AC" << endl;
    } else {
        cout << "WA" << endl;
    }
}

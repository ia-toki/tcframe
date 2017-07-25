#include <bits/stdc++.h>
using namespace std;

int ac() {
    cerr << "AC" << endl;
    return 0;
}

int wa() {
    cerr << "WA" << endl;
    return 0;
}

int main(int argc, char** argv) {
    ifstream tc_in(argv[1]);

    int N;
    tc_in >> N;

    int guesses_count = 0;

    while (true) {
        int guess;
        cin >> guess;

        guesses_count++;

        if (guesses_count > 5) {
            return wa();
        } else if (guess < N) {
            cout << "TOO_SMALL" << endl;
        } else if (guess > N){
            cout << "TOO_LARGE" << endl;
        } else {
            return ac();
        }
    }
}

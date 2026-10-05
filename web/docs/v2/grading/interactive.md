# Interactive Problems

In an interactive problem the solution talks to a **communicator** program during the run. The communicator sends questions and reads answers, and it decides the verdict.

```cpp
void StyleConfig() {
    InteractiveEvaluator();
}
```

An interactive problem doesn't have expected `.out` files, so `NoOutput()` is implied.

## How the pieces are connected

For each test case, the runner starts the communicator with the test case input file as its argument. The two programs are connected in both directions:

- Whatever the solution prints goes to the communicator's stdin.
- Whatever the communicator prints goes to the solution's stdin.

## Verdict

The communicator writes its verdict to **stderr** as the first line, for example `AC` or `WA`. The runner uses that line as the test case verdict. If the solution times out or crashes, the runner reports `TLE` or `RTE` instead.

## Example communicator

This communicator hides a number between 1 and 10. It accepts up to five guesses.

```cpp
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    ifstream tc_in(argv[1]);
    int N;
    tc_in >> N;

    int guesses = 0;
    while (true) {
        int guess;
        cin >> guess;
        guesses++;

        if (guesses > 5) {
            cerr << "WA" << endl;
            return 0;
        } else if (guess < N) {
            cout << "TOO_SMALL" << endl;
        } else if (guess > N) {
            cout << "TOO_LARGE" << endl;
        } else {
            cerr << "AC" << endl;
            return 0;
        }
    }
}
```

The solution for this example guesses by binary search:

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    int lo = 1, hi = 10;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        cout << mid << endl;

        string response;
        cin >> response;
        if (response == "TOO_SMALL") lo = mid + 1;
        else if (response == "TOO_LARGE") hi = mid - 1;
        else break;
    }
}
```

## Grading

Pass the communicator as a source file, and it is compiled for you:

```sh
tcframe test --communicator=communicator.cpp
tcframe grade --solution=./sol --communicator=communicator.cpp
```

Put the communicator's source in the package folder, or pass its path.

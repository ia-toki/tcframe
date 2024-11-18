# Helper programs

TCFrame's [local grading](../topic-guides/grading) system allows the use of **helper** programs, which includes scorer and communicator programs. This is the format that complies with TCFrame's local grading system. If your grading system expects a different format, please adjust the helper programs before setting up the problem there.

## Scorer

A scorer reads the contestant's output and then outputs the verdict.

|Input||
|-|-|
|argv[1]|test case input filename|
|argv[2]|test case output filename|
|argv[3]|contestant's output filename|

|Output||
|-|-|
|stdout|verdict (see the [format](#verdict-format) in the next section)|

Here is an example scorer which gives AC if the contestant's output differs not more than 1e-9 from the official output.

```cpp title="scorer.cpp"
#include <bits/stdc++.h>
using namespace std;

int wa() {
    cout << "WA" << endl;
    return 0;
}

int ac() {
    cout << "AC" << endl;
    return 0;
}

int main(int argc, char* argv[]) {
    ifstream tc_in(argv[1]);
    ifstream tc_out(argv[2]);
    ifstream con_out(argv[3]);

    double tc_ans;
    tc_out >> tc_ans;

    double con_ans;
    if (!(con_out >> con_ans)) {
        return wa();
    }

    if (abs(tc_ans - con_ans) < 1e-9) {
        return ac();
    } else {
        return wa();
    }
}
```

---

## Communicator

A communicator communicates with the contestant's solution program via stdin/stdout, and then outputs the verdict to stderr.

|Input||
|-|-|
|argv[1]|test case input filename|
|stdin|input from contestant's program|

|Output||
|-|-|
|stdout|output to contestant's program|
|stderr|verdict (see the [format](#verdict-format) in the next section)|

Make sure the communicator flushes after every time it writes output.

Here is an example communicator program in a typical binary search problem.

```cpp title="communicator.cpp"
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

int main(int argc, char* argv[]) {
    ifstream tc_in(argv[1]);

    int N;
    tc_in >> N;

    int guesses_count = 0;

    while (true) {
        int guess;
        cin >> guess;

        guesses_count++;

        if (guesses_count > 10) {
            return wa();
        } else if (guess < N) {
            cout << "TOO_SMALL" << endl;
        } else if (guess > N) {
            cout << "TOO_LARGE" << endl;
        } else {
            return ac();
        }
    }
}
```

---

## Verdict format

A scorer and communicator should output the verdict of a test case in the following format:

|Verdict|Meaning|
|-|-|
|<pre>AC</pre>|The solution receives full points.|
|<pre>WA</pre>|The solution receives zero points.|
|<pre>OK<br/>X</pre>|The solution receives partial points, which is `X`.<br/>`X` can be a floating point.<br/>E.g.:<br/><pre>OK<br/>71</pre>|
|<pre>OK<br/>P%</pre>|The solution receives partial points, which is `P`% of the full points.<br/>E.g.:<br/><pre>OK<br/>50%</pre>|
